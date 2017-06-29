#include <string.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "tasks.hpp"
#include "examples/examples.hpp"
#include "periodic_scheduler/periodic_callback.h"
#include "uart2.hpp"
#include "uart3.hpp"
#include "utilities.h"
#include "uart0_min.h"
#include "io.hpp"
#include "storage.hpp"
#include "tlm/c_tlm_var.h"

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define AVERAGE_SIZE 100
#define BUFFER_SIZE 512
#define TEMP_BUFFER_SIZE 64
#define BIT_0   ( 1 << 0 )
#define BIT_4   ( 1 << 4 )
#define DEBUG false

QueueHandle_t xSensorQueue;
EventGroupHandle_t xWatchdogFlags;
SemaphoreHandle_t xMutexSPI;

tlm_component * bucket;

uint32_t xDelay = 100;
uint32_t Time = 0;

void AppendCPUInfoToString(char * Buffer)
{
    // Note:  If there is a linker error and the FreeRTOS function is not included, just
    // hack tasks.c at FreeRTOS source code and force include uxTaskGetSystemState()
    char Temp[TEMP_BUFFER_SIZE] = { 0 };

    // Enum to char : eRunning, eReady, eBlocked, eSuspended, eDeleted
    const char * const taskStatusTbl[] = { "RUN", "RDY", "BLK", "SUS", "DEL" };

    // Limit the tasks to avoid heap allocation.
    const unsigned portBASE_TYPE maxTasks = 16;
    TaskStatus_t status[maxTasks];
    uint32_t totalRunTime = 0;
    uint32_t tasksRunTime = 0;
    const unsigned portBASE_TYPE uxArraySize = uxTaskGetSystemState(&status[0], maxTasks, &totalRunTime);

    sprintf(Temp, "Current Runtime Time = %d\n", Time);
    strcat(Buffer, Temp);
    sprintf(Temp, "%10s Sta Pr Stack CPU%%          Time\n", "Name");
    strcat(Buffer, Temp);
    for(unsigned priorityNum = 0; priorityNum < configMAX_PRIORITIES; priorityNum++)
    {
        /* Print in sorted priority order */
        for (unsigned i = 0; i < uxArraySize; i++)
        {
            TaskStatus_t *e = &status[i];
            if (e->uxBasePriority == priorityNum)
            {
                tasksRunTime += e->ulRunTimeCounter;

                const uint32_t cpuPercent = (0 == totalRunTime) ? 0 : e->ulRunTimeCounter / (totalRunTime/100);
                const uint32_t timeUs = e->ulRunTimeCounter;
                const uint32_t stackInBytes = (4 * e->usStackHighWaterMark);

                sprintf(Temp, "%10s %s %2u %5u %4u %10u us\n",
                              e->pcTaskName, taskStatusTbl[e->eCurrentState], e->uxBasePriority,
                              stackInBytes, cpuPercent, timeUs);
                strcat(Buffer, Temp);
            }
        }
    }

     // Overhead is the time not accounted towards any of the tasks.
     // * For example, when an ISR happens, that is not part of a task's CPU usage.

    const uint32_t overheadUs = (totalRunTime - tasksRunTime);
    const uint32_t overheadPercent = overheadUs / (totalRunTime / 100);
    sprintf(Temp, "%10s --- -- ----- %4u %10u uS\n",
                  "(overhead)", overheadPercent, overheadUs);

    strcat(Buffer, Temp);

    if (uxTaskGetNumberOfTasks() > maxTasks)
    {
        sprintf(Temp, "** WARNING: Only reported first %u tasks\n", maxTasks);
        strcat(Buffer, Temp);
    }
}

inline void Debug_Print(const char * str)
{
    if(DEBUG)
    {
        uart0_puts(str);
    }
}

class vCounterTask : public scheduler_task
{
    public:
        vCounterTask(uint8_t priority) : scheduler_task("vCounterTask", 2048, priority) {}
        bool init(void)
        {
            xLastWakeTime = xTaskGetTickCount();
            xFrequency = 1;
            Time = 0;
            TLM_REG_VAR(bucket, Time, tlm_uint);
            return true;
        }
        bool run(void *p)
        {
            vTaskDelayUntil(&xLastWakeTime, xFrequency);
            Time++;
            return true;
        }
    private:
        TickType_t xFrequency;
        TickType_t xLastWakeTime;
};

class vWatchdogTask : public scheduler_task
{
    public:
        vWatchdogTask(uint8_t priority) : scheduler_task("vWatchdogTask", 2048, priority) {}
        bool init(void)
        {
            TLM_REG_VAR(bucket, uxBits, tlm_uint);
            Counter = 0;
            Previous_Time = 0;
            Temp[0] = 0;
            CPU_Buffer[0] = 0;
            STUCK_Buffer[0] = 0;
            uxBits_Previous = (BIT_4 | BIT_0);
            Storage::write("1:cpu.txt", (void *)"CPU Log\n", 8, 0);
            Storage::write("1:stuck.txt", (void *)"Time, Task #\n", 13, 0);
            return true;
        }
        bool run(void *p)
        {
            uint32_t Current_Time = Time;

            uxBits = xEventGroupWaitBits(
                xWatchdogFlags,     /* The event group being tested. */
                (BIT_4 | BIT_0),    /* The bits within the event group to wait for. */
                pdFALSE,             /* BIT_0 & BIT_4 should be cleared before returning. */
                pdTRUE,             /* Wait for both bits. */
                1000                /* Timeout of 1 second*/
            );

            if(uxBits == ( BIT_0 | BIT_4 ))
            {
                /* xEventGroupWaitBits() returned because both bits were set. */
            }
            else if(!CHECK_BIT(uxBits, 0) && !CHECK_BIT(uxBits, 4))
            {
                sprintf(Temp, "%d, 1\n%d, 2\n", Time, Time);
            }
            else if(CHECK_BIT(uxBits, 0))
            {
                sprintf(Temp, "%d, 1\n", Time);
            }
            else if(CHECK_BIT(uxBits, 4))
            {
                sprintf(Temp, "%d, 2\n", Time);
            }

            //// If the Bits are different
            if(uxBits_Previous != uxBits)
            {
                printf("Task Bits Change: 0x%02X\n", uxBits);
                uxBits_Previous = uxBits;
            }

            if(uxBits != ( BIT_4 | BIT_0 ))
            {
                strcat(STUCK_Buffer, Temp);
                if(xSemaphoreTake(xMutexSPI, portMAX_DELAY) == pdTRUE)
                {
                    Storage::append("1:stuck.txt", STUCK_Buffer, strlen(STUCK_Buffer), 0);
                    xSemaphoreGive(xMutexSPI);
                    STUCK_Buffer[0] = 0;
                }
            }

            xEventGroupClearBits(xWatchdogFlags, (BIT_0 | BIT_4));

            if((Current_Time-Previous_Time) >= 60000)
            {
                Previous_Time = Current_Time;

                AppendCPUInfoToString(CPU_Buffer);
                if(xSemaphoreTake(xMutexSPI, portMAX_DELAY) == pdTRUE)
                {
                    printf("CPU APPENDING: %s", CPU_Buffer);
                    Storage::append("1:cpu.txt", CPU_Buffer, strlen(CPU_Buffer), 0);
                    xSemaphoreGive(xMutexSPI);
                    //// NOTE: I placed this here in the event I want to move from
                    //// portMAX_DELAY to a timeout.
                    Counter = 0;
                    CPU_Buffer[0] = 0;
                }
            }
            else
            {
                Counter++;
            }
            return true;
        }
    private:
        EventBits_t uxBits;
        EventBits_t uxBits_Previous;
        uint32_t Counter;
        uint32_t Previous_Time;
        char Temp[TEMP_BUFFER_SIZE];
        char CPU_Buffer[BUFFER_SIZE];
        char STUCK_Buffer[BUFFER_SIZE];
};

class vProducerTask : public scheduler_task
{
    public:
        vProducerTask(uint8_t priority) : scheduler_task("vProducerTask", 2048, priority) {}
        void reset(void)
        {
            Readings = 0;
            Readings_Counter = 0;
        }
        bool init(void)
        {
            reset();
            return true;
        }
        bool run(void *p)
        {
            if(Readings_Counter < AVERAGE_SIZE)
            {
                Readings += LS.getRawValue();
                Readings_Counter++;
            }
            else
            {
                Readings /= AVERAGE_SIZE;
                uint16_t Send = (uint16_t)Readings;
                if(xQueueSend(xSensorQueue, (void*)&Send, portMAX_DELAY) == pdPASS) { }
                reset();
            }
            xEventGroupSetBits(xWatchdogFlags, BIT_0);
            vTaskDelay(1);
            return true;
        }
    private:
        uint32_t Readings;
        uint16_t Readings_Counter;
};

class vConsumerTask : public scheduler_task
{
    public:
        vConsumerTask(uint8_t priority) : scheduler_task("vConsumerTask", 2048, priority) {}
        bool init(void)
        {
            Light = 0;
            Previous_Time = 0;
            Buffer[0] = 0;
            memset(Temp, 0, TEMP_BUFFER_SIZE);
            TLM_REG_VAR(bucket, Light, tlm_uint);
            Storage::write("1:sensors.txt", (void *)"Time, Sensors\n", 14, 0);
            return true;
        }
        bool run(void *p)
        {
            uint32_t Current_Time = Time;

            if(xQueueReceive(xSensorQueue, &Light, portMAX_DELAY) == pdPASS)
            {
                uint32_t Temp_Length = sprintf(Temp, "%d, %d\n", Time, Light);
                if(strlen(Buffer)+Temp_Length < BUFFER_SIZE)
                {
                    strcat(Buffer, Temp);
                }
                else
                {
                    //// Buffer full force write to file
                    printf("Consumer Buffer Full!\n");
                    Previous_Time = Current_Time-1000;
                }
            }

            if((Current_Time-Previous_Time) >= 1000)
            {
                Previous_Time = Current_Time;
                //// Write String to file
                if(xSemaphoreTake(xMutexSPI, portMAX_DELAY) == pdTRUE)
                {
                    //printf("CONSUMER APPENDING: %s", Buffer);
                    Storage::append("1:sensors.txt", Buffer, strlen(Buffer), 0);
                    xSemaphoreGive(xMutexSPI);
                }
                //// Start string with a zero to denote an empty string
                Buffer[0] = 0;
            }
            xEventGroupSetBits(xWatchdogFlags, BIT_4);
            return true;
        }
    private:
        uint16_t Light;
        uint32_t Previous_Time;
        char Buffer[BUFFER_SIZE];
        char Temp[TEMP_BUFFER_SIZE];
};

int main()
{
    bucket = tlm_component_add("bucket");

    xSensorQueue = xQueueCreate( 10, sizeof( uint16_t ) );
    xWatchdogFlags = xEventGroupCreate();
    xMutexSPI = xSemaphoreCreateMutex();

    scheduler_add_task(new terminalTask(PRIORITY_HIGH));

    scheduler_add_task(new vProducerTask(PRIORITY_MEDIUM));

    scheduler_add_task(new vConsumerTask(PRIORITY_MEDIUM));

    scheduler_add_task(new vWatchdogTask(PRIORITY_CRITICAL));

    scheduler_add_task(new vCounterTask(PRIORITY_CRITICAL));

    scheduler_start(); ///< This shouldn't return

    return -1;
}