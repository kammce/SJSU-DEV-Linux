#include <string.h>
#include <stdio.h>
#include <math.h>

#include "tasks.hpp"
#include "examples/examples.hpp"
#include "periodic_scheduler/periodic_callback.h"
#include "uart2.hpp"
#include "uart3.hpp"
#include "utilities.h"
#include "uart0_min.h"
#include "io.hpp"

#define PI 3.14159265359
#define BUFFER_SIZE 255

QueueHandle_t xOrientationQueue;

uint32_t xDelay = 100;

typedef struct
{
  double roll;
  double pitch;
} Orientation_S;

enum AbsoluteOrientation
{
    LEVEL=0,
    UP,
    DOWN,
    LEFT,
    SUPER_LEFT,
    RIGHT,
    SUPER_RIGHT
};

bool debug = false;

inline void Debug_Print(const char * str)
{
    #if(DEBUG == 1)
    {
        uart0_puts(str);
    }
    #endif
}

class vOrientationTask : public scheduler_task
{
    public:
        vOrientationTask(uint8_t priority) : scheduler_task("vOrientationTask", 2048, priority) {}
        bool init(void)
        {
            return true;
        }
        bool run(void *p)
        {
            while(1)
            {
                int tilt_x = AS.getX();
                int tilt_y = AS.getY();
                int tilt_z = AS.getZ();

                orient.roll  = (atan2(-tilt_x, tilt_z)*180.0)/PI;
                orient.pitch = (atan2(tilt_y, sqrt(tilt_x*tilt_x + tilt_z*tilt_z))*180.0)/PI;

                Debug_Print("[SENDING] Orientation");

                if (SW.getSwitch(1))
                {
                    debug = true;
                }
                else if (SW.getSwitch(2))
                {
                    debug = false;
                }
                else if (SW.getSwitch(3))
                {
                    scheduler_task *vConsumerTaskHandler = scheduler_task::getTaskPtrByName("vConsumerTask");
                    scheduler_task *vOrientationTaskkHandler = scheduler_task::getTaskPtrByName("vOrientationTask");

                    vTaskPrioritySet( vConsumerTaskHandler->getTaskHandle(), PRIORITY_MEDIUM );
                    vTaskPrioritySet( vOrientationTaskkHandler->getTaskHandle(), PRIORITY_LOW );
                }
                else if (SW.getSwitch(4))
                {
                    scheduler_task *vConsumerTaskHandler = scheduler_task::getTaskPtrByName("vConsumerTask");
                    scheduler_task *vOrientationTaskkHandler = scheduler_task::getTaskPtrByName("vOrientationTask");

                    vTaskPrioritySet( vConsumerTaskHandler->getTaskHandle(), PRIORITY_LOW );
                    vTaskPrioritySet( vOrientationTaskkHandler->getTaskHandle(), PRIORITY_MEDIUM );
                }

                Absolute_Orient = LEVEL;
                Absolute_Orient = (orient.roll < -30)  ? LEFT : Absolute_Orient;
                Absolute_Orient = (orient.roll < -60)  ? SUPER_LEFT : Absolute_Orient;
                Absolute_Orient = (orient.roll > 30)   ? RIGHT : Absolute_Orient;
                Absolute_Orient = (orient.roll > 60)   ? SUPER_RIGHT : Absolute_Orient;
                Absolute_Orient = (orient.pitch < -30) ? DOWN : Absolute_Orient;
                Absolute_Orient = (orient.pitch > 30)  ? UP : Absolute_Orient;

                if(xQueueSend(
                        xOrientationQueue,
                        ( void * ) &Absolute_Orient,
                        ( TickType_t ) 0
                    ) != pdPASS
                )
                {
                    uart0_puts("FAILED TO PUSH NEW ORIENTATION DATA TO QUEUE!!!\n");
                }

                Debug_Print("[SENT] Orientation\n");

                vTaskDelay(xDelay);
            }
        }
    private:
        Orientation_S orient;
        AbsoluteOrientation Absolute_Orient;
};

char buffer[BUFFER_SIZE];


class vConsumerTask : public scheduler_task
{
    public:
        vConsumerTask(uint8_t priority) : scheduler_task("vConsumerTask", 2048, priority) {}
        bool init(void)
        {
            return true;
        }
        bool run(void *p)
        {
            while(1)
            {

                Debug_Print("[RECIEVING] Orientation");

                // if( xQueueReceive( xOrientationQueue, &( orient ), ( TickType_t ) 10 ) )
                // {
                //     sprintf(buffer, "Roll = %f :: Pitch = %f\n", orient.roll, orient.pitch);
                //     Debug_Print(buffer);
                // }

                if( xQueueReceive(
                    xOrientationQueue,
                    &( Absolute_Orient ),
                    ( TickType_t ) 0 )
                )
                {
                    switch(Absolute_Orient)
                    {
                        case SUPER_LEFT:
                            LE.setAll(0);
                            LE.on(1);
                            break;
                        case LEFT:
                            LE.setAll(0);
                            LE.on(2);
                            break;
                        case RIGHT:
                            LE.setAll(0);
                            LE.on(3);
                            break;
                        case SUPER_RIGHT:
                            LE.setAll(0);
                            LE.on(4);
                            break;
                        default:
                            LE.setAll(0);
                            break;
                    }
                }

                Debug_Print("[RECIEVED] Orientation");
                // vTaskDelay(xDelay);
            }
        }
    private:
        AbsoluteOrientation Absolute_Orient;
};

int main()
{
    xOrientationQueue = xQueueCreate( 10, sizeof( AbsoluteOrientation ) );

    scheduler_add_task(new terminalTask(PRIORITY_HIGH));

    scheduler_add_task(new vOrientationTask(PRIORITY_MEDIUM));

    scheduler_add_task(new vConsumerTask(PRIORITY_LOW));

    scheduler_start(); ///< This shouldn't return

    return -1;
}