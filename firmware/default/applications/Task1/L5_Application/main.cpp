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

/**
 * @file
 * @brief This is the application entry point.
 * 			FreeRTOS and stdio printf is pre-configured to use uart0_min.h before main() enters.
 * 			@see L0_LowLevel/lpc_sys.h if you wish to override printf/scanf functions.
 *
 */
#include "tasks.hpp"
#include "examples/examples.hpp"
#include "utilities.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>

scheduler_task * vLEDControllerTask_Ptr;

class vLEDControllerTask : public scheduler_task
{
public:
    vLEDControllerTask() : scheduler_task("vLEDControllerTask", 2000, PRIORITY_LOW) {}
    bool init(void)
    {
        //// Select PCLK_PWM1
        LPC_SC->PCLKSEL0        |= (0b11 << 12);
        //// Pin Select PWM Channel 1.6 for P2.5
        LPC_PINCON->PINSEL4     |=  (1 << 10);
        LPC_PINCON->PINSEL4     &= ~(1 << 11);
        //// Set Prescalar to 48,000 to scale PCLK of 48MHz
        //// to 1000KHz
        LPC_PWM1->PR             = 30;
        //// Set PWM Rate to 1000 TC clocks
        LPC_PWM1->MR0            = 1000;
        //// Set Default PWM rate to half (50%)
        LPC_PWM1->MR6            = 1;
        //// Set Timer Control Register to
        ////    Enable PWM
        ////    Enable Counter
        LPC_PWM1->TCR           |= (1 << 3) | (1 << 0);
        //// Enable PWM through PWM Control Register
        LPC_PWM1->PCR           |= (1 << 14);
        return true;
    }
    bool run(void *p)
    {
        static uint32_t pwm = 0;
        static uint32_t counter = 0;
        static uint32_t ulNotifiedValue;
        enum STATE_E
        {
            OFF=0,
            BREATHING,
            PULSE_FALL
        };

        static uint8_t state = OFF;

        vTaskDelay(50);

        switch(state)
        {
            case OFF:
                pwm = 0;
                if(xTaskNotifyWait(0x0, 0x0, &ulNotifiedValue, 0) == pdTRUE)
                {
                    state = BREATHING;
                    counter = 0;
                }
                // pwm = 1000;
                // state = PULSE_FALL;
                break;
            case BREATHING:
                counter += 5;
                pwm = (uint32_t)abs(250*sin((counter)*(3.14/180)));
                printf("%d\n", pwm);
                if(xTaskNotifyWait(0x0, 0x0, &ulNotifiedValue, 0) == pdTRUE)
                {
                    pwm = 1000;
                    counter = 0;
                    state = PULSE_FALL;
                }
                break;
            case PULSE_FALL:
                counter++;
                pwm = (pwm > 0) ? pwm - 100 : 0;
                state = (pwm > 0) ? state : OFF;
                break;
        }
        LPC_PWM1->MR6 = pwm;
        LPC_PWM1->LER |= (1 << 6);
        return true;
    }
};

class lab1 : public scheduler_task
{
public:
    lab1() : scheduler_task("GPIOLab1", 2000, PRIORITY_LOW)
    {

    }
    bool init(void)
    {
        //// Set P[1][28] as output
        LPC_GPIO1->FIODIR      |= (1 << 28);
        //// Set P[1][29] as input
        LPC_GPIO1->FIODIR      &= ~(0 << 29);
        return true;
    }
    bool run(void *p)
    {
        enum STATE_E
        {
            UNPRESSED=0,
            WAITING_FOR_RELEASE,
            RELEASED
        };
        static uint8_t state = UNPRESSED;
        vTaskDelay(50);
        //printf("BUTTON RELEASED");
        //printf("S: %d :: P1.29 = %d :!: %d\n", state, (LPC_GPIO1->FIOPIN & (1 << 29)), !(LPC_GPIO1->FIOPIN & (1 << 29)));
        switch(state)
        {
            case UNPRESSED:
                state = !(LPC_GPIO1->FIOPIN & (1 << 29)) ? WAITING_FOR_RELEASE : state;
                break;
            case WAITING_FOR_RELEASE:
                state = (LPC_GPIO1->FIOPIN & (1 << 29)) ? RELEASED : state;
                break;
            case RELEASED:
                LPC_GPIO1->FIOPIN ^= (1 << 28);
                xTaskNotify(vLEDControllerTask_Ptr->getTaskHandle(), 0, eNoAction);
                state = UNPRESSED;
                break;
            default:
                break;
        }
        return true;
    }
};


/**
 * The main() creates tasks or "threads".  See the documentation of scheduler_task class at scheduler_task.hpp
 * for details.  There is a very simple example towards the beginning of this class's declaration.
 *
 * @warning SPI #1 bus usage notes (interfaced to SD & Flash):
 *      - You can read/write files from multiple tasks because it automatically goes through SPI semaphore.
 *      - If you are going to use the SPI Bus in a FreeRTOS task, you need to use the API at L4_IO/fat/spi_sem.h
 *
 * @warning SPI #0 usage notes (Nordic wireless)
 *      - This bus is more tricky to use because if FreeRTOS is not running, the RIT interrupt may use the bus.
 *      - If FreeRTOS is running, then wireless task may use it.
 *        In either case, you should avoid using this bus or interfacing to external components because
 *        there is no semaphore configured for this bus and it should be used exclusively by nordic wireless.
 */

int main(void)
{
    /**
     * A few basic tasks for this bare-bone system :
     *      1.  Terminal task provides gateway to interact with the board through UART terminal.
     *      2.  Remote task allows you to use remote control to interact with the board.
     *      3.  Wireless task responsible to receive, retry, and handle mesh network.
     *
     * Disable remote task if you are not using it.  Also, it needs SYS_CFG_ENABLE_TLM
     * such that it can save remote control codes to non-volatile memory.  IR remote
     * control codes can be learned by typing the "learn" terminal command.
     */
    scheduler_add_task(new lab1());

    vLEDControllerTask_Ptr = new vLEDControllerTask();
    scheduler_add_task(vLEDControllerTask_Ptr);

    scheduler_add_task(new terminalTask(PRIORITY_HIGH));

    /* Consumes very little CPU, but need highest priority to handle mesh network ACKs */
    scheduler_add_task(new wirelessTask(PRIORITY_CRITICAL));

    /* Change "#if 0" to "#if 1" to run period tasks; @see period_callbacks.cpp */
    #if 1
    const bool run_1Khz = false;
    scheduler_add_task(new periodicSchedulerTask(run_1Khz));
    #endif

    /* The task for the IR receiver to "learn" IR codes */
    // scheduler_add_task(new remoteTask  (PRIORITY_LOW));

    /* Your tasks should probably used PRIORITY_MEDIUM or PRIORITY_LOW because you want the terminal
     * task to always be responsive so you can poke around in case something goes wrong.
     */

    /**
     * This is a the board demonstration task that can be used to test the board.
     * This also shows you how to send a wireless packets to other boards.
     */
    #if 0
        scheduler_add_task(new example_io_demo());
    #endif

    /**
     * Change "#if 0" to "#if 1" to enable examples.
     * Try these examples one at a time.
     */
    #if 0
        scheduler_add_task(new example_task());
        scheduler_add_task(new example_alarm());
        scheduler_add_task(new example_logger_qset());
        scheduler_add_task(new example_nv_vars());
    #endif

    /**
	 * Try the rx / tx tasks together to see how they queue data to each other.
	 */
    #if 0
        scheduler_add_task(new queue_tx());
        scheduler_add_task(new queue_rx());
    #endif

    /**
     * Another example of shared handles and producer/consumer using a queue.
     * In this example, producer will produce as fast as the consumer can consume.
     */
    #if 0
        scheduler_add_task(new producer());
        scheduler_add_task(new consumer());
    #endif

    /**
     * If you have RN-XV on your board, you can connect to Wifi using this task.
     * This does two things for us:
     *   1.  The task allows us to perform HTTP web requests (@see wifiTask)
     *   2.  Terminal task can accept commands from TCP/IP through Wifly module.
     *
     * To add terminal command channel, add this at terminal.cpp :: taskEntry() function:
     * @code
     *     // Assuming Wifly is on Uart3
     *     addCommandChannel(Uart3::getInstance(), false);
     * @endcode
     */
    #if 0
        Uart3 &u3 = Uart3::getInstance();
        u3.init(WIFI_BAUD_RATE, WIFI_RXQ_SIZE, WIFI_TXQ_SIZE);
        scheduler_add_task(new wifiTask(Uart3::getInstance(), PRIORITY_LOW));
    #endif

    scheduler_start(); ///< This shouldn't return
    return -1;
}
