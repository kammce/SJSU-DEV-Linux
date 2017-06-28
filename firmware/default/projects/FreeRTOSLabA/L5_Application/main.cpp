#include "tasks.hpp"
#include "examples/examples.hpp"
#include "periodic_scheduler/periodic_callback.h"
#include "uart2.hpp"
#include "uart3.hpp"
#include "utilities.h"
#include <string.h>
#include "uart0_min.h"
#include "io.hpp"

#define STRING_SIZE 100

char a[STRING_SIZE], b[STRING_SIZE];

TaskHandle_t xTaskA, xTaskB, xTaskInput = NULL;

uint32_t xDelay = 50;

void TaskInput(void* p)
{
    bool pressed[5] = {0};
    while(1)
    {
        for (int i = 1; i < 5; ++i)
        {
            if(SW.getSwitch(i))
            {
                pressed[i] = true;
            }
            else if(pressed[i] && !SW.getSwitch(i))
            {
                switch(i)
                {
                    case 1:
                        uart0_puts("Button 0 Pressed, xDelay = 1000\n");
                        xDelay = 1000;
                        break;
                    case 2:
                        uart0_puts("Button 1 Pressed, xDelay = 50\n");
                        xDelay = 50;
                        break;
                    case 3:
                        uart0_puts("Button 2 Pressed, TaskA = 1, TaskB = 2\n");
                        vTaskPrioritySet( xTaskA, 1 );
                        vTaskPrioritySet( xTaskB, 2 );
                        break;
                    case 4:
                        uart0_puts("Button 3 Pressed, TaskA = 3, TaskB = 3\n");
                        vTaskPrioritySet( xTaskA, 3 );
                        vTaskPrioritySet( xTaskB, 3 );
                        break;
                }
                pressed[i] = false;
            }
            vTaskDelay(50);
        }
    }
}

void TaskA(void* p)
{
    while(1)
    {
        uart0_puts(a);
        vTaskDelay(xDelay);
    }
}

void TaskB(void* p)
{
    while(1)
    {
        uart0_puts(b);
        vTaskDelay(xDelay);
    }
}

int main()
{
    memset(a, 'a', STRING_SIZE-1);
    memset(b, 'b', STRING_SIZE-1);

    xTaskCreate(
        TaskInput,
        "TaskA",
        STACK_BYTES(2048),
        0,
        10,
        &xTaskInput
    );
    xTaskCreate(
        TaskA,
        "TaskA",
        STACK_BYTES(2048),
        0,
        1,
        &xTaskA
    );
    xTaskCreate(
        TaskB,
        "TaskB",
        STACK_BYTES(2048),
        0,
        1,
        &xTaskB
    );
    vTaskStartScheduler();

    return -1;
}