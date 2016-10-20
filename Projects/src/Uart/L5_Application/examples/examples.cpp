/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "examples.hpp"
#include "rtc_alarm.h"
#include "rtc.h"
#include "file_logger.h"
#include "io.hpp"
#include "c_tlm_var.h"
#include "io.hpp"
#include "wireless.h"
#include "uart0.hpp"
#include "uart2.hpp"
#include "uart3.hpp"


#define TIME_SLICE 10
#define GRAVITY 9.8f
#define BIT_SENSITIVITY 1024.0f
#define WEIGHT 0.05f
#define MILLIS_PER_SECOND 1000

ComTask::ComTask() : scheduler_task("Communication Task", 3 * 512, PRIORITY_LOW)
{
    setRunDuration(TIME_SLICE);
}

bool ComTask::init(void *p)
{
    return true;
}
bool ComTask::run(void *p)
{
    Uart2& u2 = Uart2::getInstance();
    Uart3& u3 = Uart3::getInstance();
    Acceleration_Sensor& acc = Acceleration_Sensor::getInstance();

    // Distance in mm
    static double X_Position = 0;
    static int X_Position_Delta = 0;
    static double X_Velocity = 0;
    static double X_Acceleration_lowpass = 0;
    static bool Acceleration_Trigger = false;
    static uint8_t counter = 0;

    // double Z_Acceleration = (acc.getZ()*GRAVITY)/BIT_SENSITIVITY;
    // double Y_Acceleration = (acc.getY()*GRAVITY)/BIT_SENSITIVITY;
    double X_Acceleration = (acc.getX()*GRAVITY)/BIT_SENSITIVITY;


    if(!Acceleration_Trigger && fabs(X_Acceleration) > 1.2)
    {
        Acceleration_Trigger = true;
    }
    else if(Acceleration_Trigger && counter < 50)
    {
        counter++;
        X_Acceleration -= X_Acceleration_lowpass;
        double ms_time = TIME_SLICE/1000.0f;

        X_Velocity = (WEIGHT * X_Velocity) + (X_Acceleration * ms_time);
        X_Position = X_Position + (X_Velocity * ms_time);

    }
    else if(counter >= 50)
    {
        //X_Position = 0;
        X_Velocity = 0;
        X_Acceleration_lowpass = 0;
        counter = 0;
        Acceleration_Trigger = false;
    } else {
        X_Acceleration_lowpass = ((WEIGHT)*X_Acceleration)+((1-WEIGHT)*X_Acceleration_lowpass);
    }

    //printf("P=%.3fm::V=%.6fm/s::A=%.6fm/s^2::LOW=%.6fm/s^2\n", X_Position*10000, X_Velocity, X_Acceleration, X_Acceleration_lowpass);

    char Recieved_Character = 'A';
    bool UART_Status = u2.getChar(&Recieved_Character, 0);
    if(UART_Status) {
        printf("%c:%d\n", Recieved_Character, UART_Status);
        switch(Recieved_Character) {
            case '1':
            case '2':
            case '3':
            case '4':
                LE.toggle(Recieved_Character-'0');
                break;
            case '5':
                // int16_t X = acc.getX();
                // int16_t Z = acc.getZ();
                // double pitch = (180 * (atan(Z/sqrt((X*X)+(Z*Z)))/M_PI));
                //u3.printf("Position = %.3f\n", X_Position);
                X_Position_Delta = (int)fabs((fabs(X_Position*10000)-fabs(X_Position_Delta)));
                u2.printf("%d\n", (int)X_Position_Delta);
                printf("Position = %d\n", X_Position_Delta);
                LD.setNumber((int)X_Position_Delta);
                break;
        }
    }
    return true;
}

ComTask2::ComTask2() : scheduler_task("Communication Task 2", 3 * 512, PRIORITY_LOW)
{
    LD.setNumber(0);
    setRunDuration(500);
}

bool ComTask2::init(void *p)
{
    return true;
}
bool ComTask2::run(void *p)
{
    //===================================
    // UART 3 Recieve
    //===================================
    const int BUFFER_SIZE = 255;
    static char Buffer[BUFFER_SIZE] = {0};
    static uint8_t Buffer_Position = 0;
    static int Position = 0;

    Uart3& u3 = Uart3::getInstance();

    char Recieved_Character = 'A';
    bool UART_Status = u3.getChar(&Recieved_Character, 0);
    if(UART_Status) {
        printf("%c:%d\n", Recieved_Character, UART_Status);
        switch(Recieved_Character) {
            case '\n':
                sscanf(Buffer, "Pos = %d", &Position);
                LD.setNumber((int)Position);
                printf("%s\n", Buffer);
                memset(Buffer, 0, sizeof(Buffer));
                Buffer_Position = 0;
                break;
            default:
                Buffer[Buffer_Position++] = Recieved_Character;
                break;
        }
    }
    return true;
}

example_task::example_task() :
    scheduler_task("ex_simple", 3 * 512, PRIORITY_HIGH)
{
    /**
     * This is optional such that run() will be called every 3000ms.
     * If you don't do this, run() will be called without a delay.
     */
    setRunDuration(3000);
}

bool example_task::run(void *p)
{
    printf("\n---------------------------------\n"
               "Status Report: \n");

    printf("Temperature: %i F\n", (int)TS.getFarenheit());
    printf("CPU Usage : %i %%\n", getTaskCpuPercent());    /* get OUR tasks' cpu usage */
    printf("Free stack: %i bytes\n", (int)getFreeStack()); /* get number of bytes of free stack of our task */
    LE.on(4);
    LE.on(3);

    for(volatile int i=0; i<0xfffff; i++) {
        ; // Do dummy work
    }

    return true;
}

example_io_demo::example_io_demo() :
        scheduler_task("ex_demo", 4 * 512, PRIORITY_LOW)
{
    setRunDuration(100);
}

bool example_io_demo::run(void *p)
{
    static uint8_t pkt_counter = 0;
    const uint8_t switches = SW.getSwitchValues();
    mesh_packet_t pkt;

    enum {
        sw1 = (1 << 0),
        sw2 = (1 << 1),
        sw3 = (1 << 2),
        sw4 = (1 << 3),
    };

    switch(switches)
    {
        case sw1 :
            printf("Acceleration: %4i %4i %4i\n", AS.getX(), AS.getY(), AS.getZ());
            LD.setNumber(AS.getX());
            break;

        case sw2 :
            printf("Light : %4d\n", LS.getRawValue());
            LD.setNumber(LS.getRawValue());
            break;

        case sw3 :
            printf("Temperature: %i\n", (int) TS.getFarenheit());
            LD.setNumber(TS.getFarenheit());
            break;

        case sw4 :
            /* Send broadcast message, and increment led number if we get a packet back */
            if (!wireless_send(MESH_BROADCAST_ADDR, mesh_pkt_nack, (char*)&pkt_counter, 1, 0)) {
                puts("Failed to send packet");
            }
            else if (wireless_get_rx_pkt(&pkt, 500)) {
                LD.setNumber(pkt_counter++);
            }
            else {
                puts("Broadcast message not received!");
            }
            break;

        default :
            break;
    }

    /* Only set LEDs when one of the switches are pressed, or LEDs are ON
     * If LEDs are on, this will turn them off.
     */
    if(switches || 0 != LE.getValues()) {
        LE.setAll(switches);
    }

    /* If we receive a broadcast message with 1 byte, send something back */
    if (wireless_get_rx_pkt(&pkt, 0) && 1 == pkt.info.data_len) {
        wireless_send(MESH_BROADCAST_ADDR, mesh_pkt_nack, "ack", 3, 0);
        pkt_counter = pkt.data[0];
        LD.setNumber(pkt_counter);
    }

    return true;
}





/**
 * This example shows how to setup alarms in the form of semaphore
 * that can be given every second, minute, hour etc.
 */
example_alarm::example_alarm() :
    scheduler_task("ex_alarm", 4 * 512, PRIORITY_LOW),
    mAlarmSec(NULL),
    mAlarmMin(NULL)
{

}

bool example_alarm::init(void)
{
    mAlarmSec = xSemaphoreCreateBinary();
    mAlarmMin = xSemaphoreCreateBinary();

    return (mAlarmSec != NULL && mAlarmMin != NULL);
}

bool example_alarm::taskEntry(void)
{
    rtc_alarm_create_recurring(everySecond, &mAlarmSec);
    rtc_alarm_create_recurring(everyMinute, &mAlarmMin);

    return true;
}

bool example_alarm::run(void *p)
{
    static int print_first_sixty_secs = 0;
    ++print_first_sixty_secs;

    /**
     * Note that we use MAX delay while checking for mAlarmSec because if a second
     * hasn't elapsed, then by definition, minute won't elapse.
     */
    if (xSemaphoreTake(mAlarmSec, portMAX_DELAY)) {
        if (print_first_sixty_secs <= 60) {
            printf("tick %i -- ", (int)rtc_gettime().sec);
        }
    }
    if (xSemaphoreTake(mAlarmMin, 0)) {
        puts("A minute has elapsed!");
    }

    return true;
}






/**
 * Queueset example shows how to use FreeRTOS to block on MULTIPLE semaphores or queues.
 * The idea is that we want to call our run() when EITHER of mSec, or mMin semaphore is ready.
 * This example also shows how to log information to "log" file on flash memory.
 */
example_logger_qset::example_logger_qset() :
    scheduler_task("ex_log_qset", 4 * 512, PRIORITY_LOW),
    mSec(NULL),
    mMin(NULL)
{
    mSec = xSemaphoreCreateBinary();
    mMin = xSemaphoreCreateBinary();
}

bool example_logger_qset::init(void)
{
    /**
     * We want to block on both seconds and minutes, but any queues or semaphores
     * must be empty before adding to the queue set.  Binary sempaphore is not
     * empty after creating, so perform xSemaphoreTake()
     */
    xSemaphoreTake(mSec, 0);
    xSemaphoreTake(mMin, 0);
    initQueueSet(2, 2, mSec, mMin);

    /* Hook up semaphores to be given every second and every minute */
    rtc_alarm_create_recurring(everySecond, &mSec);
    rtc_alarm_create_recurring(everyMinute, &mMin);

    /**
     * Queue Set will use this block time, and only call run() if either
     * mSec or mMin are given or when this timeout occurs.
     */
    setQueueSetBlockTime(portMAX_DELAY);

    return (mSec != NULL && mMin != NULL);
}

bool example_logger_qset::run(void *p)
{
    static int count = 0;

    /* Did the run() get called due to the mSec semaphore? */
    if (getQueueSetSelection() == mSec) {
        // This must work but just in case, return "false" to stop this task.
        if(!xSemaphoreTake(mSec, 0)) {
            return false;
        }

        LOG_INFO("Example log info");
    }

    /* Did the run() get called due to the mMin semaphore? */
    if(getQueueSetSelection() == mMin) {
        if(!xSemaphoreTake(mMin, 0)) {
            return false;
        }
        LOG_WARN("Example log warning");
    }

    /**
     * When count exceeds 60, we stop this task on purpose
     * We flush the remaining logger buffer, and return false to suspend the task.
     */
    if (count++ >= 60) {
        LOG_FLUSH();
        puts("Information was logged to a file.  Type 'cat log' to see it.");
        puts("Suspending this task on purpose");
        return false;
    }
    else {
        return true;
    }
}






/**
 * This example shows how you can save your variables to flash memory such that
 * when the program or the board reboots, it will retrieve previous values.
 *
 * Be sure to set SYS_CFG_ENABLE_TLM to non-zero at sys_config.h
 */
example_nv_vars::example_nv_vars() :
    scheduler_task("nv_vars", 2 * 512, PRIORITY_LOW)
{
    mVarWeDontWantToLose = 0;
}

bool example_nv_vars::regTlm(void)
{
    #if SYS_CFG_ENABLE_TLM
        // Get the "disk" component list, this list is saved on disk periodically when variables change value
        tlm_component *disk = tlm_component_get_by_name(SYS_CFG_DISK_TLM_NAME);

        // Register the variable as "disk variable" whose value will be saved
        TLM_REG_VAR(disk, mVarWeDontWantToLose, tlm_int);
    #endif

    // Notice that after about 2 min of no terminal command, a message will print something like :
    // "disk variables changed, changes saved to disk"
    // Even if you reboot the board, this variable will not start from 0, but from the value
    // that was saved to the disk.
    return true;
}

bool example_nv_vars::run(void *p)
{
    // Change the variable every 60 seconds :
    mVarWeDontWantToLose++;
    vTaskDelay(60 * 1000);

    return true;
}






/**
 * This example shows how two tasks can exchange information with each other using a queue.
 *      addSharedObject() adds a pointer to be shared, which we use to share queue handle.
 *      getSharedObject() gets the pointer which all tasks can obtain.
 */
queue_tx::queue_tx() :
    scheduler_task("ex_qtx", 2 * 512, PRIORITY_LOW)
{
    /* Nothing to do */
}

bool queue_tx::init(void)
{
    // call our run() every second :
    setRunDuration(1000);

    /**
     * Create a queue, and add to mShared object by the name of "my_queue"
     * One small error is not to check for NULL from xQueueCreate()
     */
    QueueHandle_t my_queue = xQueueCreate(1, sizeof(int));
    return addSharedObject("my_queue", my_queue);
}

bool queue_tx::run(void *p)
{
    // Send only 10 samples, then suspend both tasks.
    static int sample = 0;

    // Get the pointer of the "ex_qrx" task
    scheduler_task *qrx_task = getTaskPtrByName("ex_qrx");
    QueueHandle_t queue_handle = getSharedObject("my_queue");

    if (!qrx_task || !queue_handle) {
        return false;
    }

    // Suspend the other task and ourselves after 10 samples
    if (++sample > 10) {
        qrx_task->suspend();
        suspend();
    }

    // Send to "my_queue" our integer
    return xQueueSend(queue_handle, &sample, 2000);
}

/**
 * Simple task that is receiving data from queue_tx class.
 * Since queue is already created by queue_tx::init(), we just use it in our run().
 */
queue_rx::queue_rx() :
    scheduler_task("ex_qrx", 3 * 512, PRIORITY_LOW)
{
    /* Nothing to do */
}

bool queue_rx::run(void *p)
{
    int sample = 0;
    QueueHandle_t queue_handle = getSharedObject("my_queue");

    if (!queue_handle) {
        return false;
    }

    if (xQueueReceive(queue_handle, &sample, portMAX_DELAY)) {
        printf("Got %i from queue", sample);
    }

    return true;
}






producer::producer() :
    scheduler_task("producer", 3 * 512, PRIORITY_LOW)
{
    /* Create the queue, and share the handle using an ID (enum) or index */
    QueueHandle_t qh = xQueueCreate(1, sizeof(int));
    addSharedObject(shared_SensorQueue, qh);
}

bool producer::run(void *p)
{
    /**
     * Note that we produce the data as fast as possible.  It is the queue size
     * that limits our production.  When the queue is full, our task will sleep
     * because it can no longer queue additional data due to MAX_DELAY block time.
     */
    int data = AS.getX();
    QueueHandle_t qh = getSharedObject(shared_SensorQueue);

    return xQueueSend(qh, &data, portMAX_DELAY);
}

consumer::consumer() :
    scheduler_task("consumer", 3 * 512, PRIORITY_LOW)
{
    /* We will consume data every 500ms since our run() will be called every 500ms */
    setRunDuration(500);
}

bool consumer::run(void *p)
{
    int data = 0;

    /* We should never block here by using portMAX_DELAY because the producer should
     * produce all the time.  In fact, it is waiting for us to pull an item from the
     * queue and it is already "sleeping" until the queue space is available.
     */
    xQueueReceive(getSharedObject(shared_SensorQueue), &data, portMAX_DELAY);
    printf("Acceleration sensor X-Axis: %i\n", data);

    return true;
}
