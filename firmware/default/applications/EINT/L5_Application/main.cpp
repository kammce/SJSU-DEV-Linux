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

scheduler_task * vInterruptChecker_Ptr;

// SemaphoreHandle_t xSemaphore20;
// SemaphoreHandle_t xSemaphore21;
// SemaphoreHandle_t xSemaphore01;

uint32_t status = 0;

typedef enum
{
  RISING = 0,
  FALLING,
  BOTH
} InterruptCondition_E;

#define PORT_COUNT 2
#define PIN_COUNT 32

typedef void (*FunctPtr)();

LPC_GPIO_TypeDef * GPIORegArray[PORT_COUNT] = {LPC_GPIO0, LPC_GPIO2};
__IO uint32_t * GPIOIntRegArray[PORT_COUNT][2] = {
    { &LPC_GPIOINT->IO0IntEnR, &LPC_GPIOINT->IO0IntEnF },
    { &LPC_GPIOINT->IO2IntEnR, &LPC_GPIOINT->IO2IntEnF }
};
__IO uint32_t * GPIOIntStatusRegArray[PORT_COUNT][2] = {
    { &LPC_GPIOINT->IO0IntStatR, &LPC_GPIOINT->IO0IntStatF },
    { &LPC_GPIOINT->IO2IntStatR, &LPC_GPIOINT->IO2IntStatF }
};
__IO uint32_t * GPIOIntClrRegArray[PORT_COUNT] = { &LPC_GPIOINT->IO0IntClr, &LPC_GPIOINT->IO2IntClr };

class KEINT_C
{
  private:
    static FunctPtr ISR[PORT_COUNT][PIN_COUNT];
  public:
    KEINT_C();
    bool attachInterruptHandler(uint8_t, uint32_t, void (*)(void), InterruptCondition_E);
    static void externalIRQHandler();
};

KEINT_C::KEINT_C()
{
    for (int i = 0; i < PORT_COUNT; ++i)
    {
        for (int j = 0; j < PIN_COUNT; ++j)
        {
            ISR[i][j] = 0;
        }
    }

    isr_register(EINT3_IRQn, externalIRQHandler);
    NVIC_EnableIRQ(EINT3_IRQn);
}

FunctPtr KEINT_C::ISR[PORT_COUNT][PIN_COUNT] = { 0 };

bool KEINT_C::attachInterruptHandler(
    uint8_t Port,
    uint32_t Pin,
    void (*InterruptHandler)(void),
    InterruptCondition_E Condition
)
{
    bool success = false;
    if(Pin < PIN_COUNT && (Port == 0 || Port == 2))
    {
        Port                          = (Port == 2) ? 1 : Port;
        GPIORegArray[Port]->FIODIR   &= ~(1 << Pin);
        *GPIOIntRegArray[Port][0]    |=  (Condition == BOTH || Condition == RISING ) ? (1 << Pin) : 0;
        *GPIOIntRegArray[Port][1]    |=  (Condition == BOTH || Condition == FALLING) ? (1 << Pin) : 0;
        ISR[Port][Pin]                =  InterruptHandler;
        success = true;
    }
    return success;
}

void KEINT_C::externalIRQHandler()
{
    for (int Port = 0; Port < PORT_COUNT; ++Port)
    {
        for (int Pin = 0; Pin < PIN_COUNT; ++Pin)
        {
            if( *GPIOIntStatusRegArray[Port][0] & (1 << Pin) ||
                *GPIOIntStatusRegArray[Port][1] & (1 << Pin) )
            {
                ISR[Port][Pin]();
                *GPIOIntClrRegArray[Port] |= (1 << Pin);
            }
        }
    }
}

void Port20Trigger(void)
{
    xTaskNotifyFromISR(vInterruptChecker_Ptr->getTaskHandle(), 0x20, eSetValueWithOverwrite, NULL);
    // xSemaphoreGiveFromISR(xSemaphore20, NULL);
}
void Port21Trigger(void)
{
    xTaskNotifyFromISR(vInterruptChecker_Ptr->getTaskHandle(), 0x21, eSetValueWithOverwrite, NULL);
    // xSemaphoreGiveFromISR(xSemaphore21, NULL);
}
void Port01Trigger(void)
{
    xTaskNotifyFromISR(vInterruptChecker_Ptr->getTaskHandle(), 0x01, eSetValueWithOverwrite, NULL);
    // xSemaphoreGiveFromISR(xSemaphore01, NULL);
}

class InterruptChecker : public scheduler_task
{
    KEINT_C KEINT;
public:
    InterruptChecker() : scheduler_task("InterruptChecker", 2000, PRIORITY_HIGH) { }
    bool init(void)
    {
        KEINT.attachInterruptHandler(2, 0, Port20Trigger, RISING);
        KEINT.attachInterruptHandler(2, 1, Port21Trigger, FALLING);
        KEINT.attachInterruptHandler(0, 1, Port01Trigger, BOTH);

        return true;
    }
    //// ankita
    //// shilpa
    bool run(void *p)
    {
        uint32_t ulNotifiedValue = 0;

        if(xTaskNotifyWait(0x0, 0x0, &ulNotifiedValue, 5) == pdTRUE)
        {
            printf("Interrupt Task Notify 0x%X Occured!\n", ulNotifiedValue);
        }
        // if(xSemaphoreTake( xSemaphore20, 0 ) == pdTRUE)
        // {
        //     printf("Interrupt Semaphore 0x20 Occured!\n");
        //     vTaskDelay(200);
        // }
        // if(xSemaphoreTake( xSemaphore21, 0 ) == pdTRUE)
        // {
        //     printf("Interrupt Semaphore 0x21 Occured!\n");
        //     vTaskDelay(200);
        // }
        // if(xSemaphoreTake( xSemaphore01, 0 ) == pdTRUE)
        // {
        //     printf("Interrupt Semaphore 0x01 Occured!\n");
        //     vTaskDelay(200);
        // }
        return true;
    }
};

int main(void)
{
    // xSemaphore20 = xSemaphoreCreateBinary();
    // xSemaphore21 = xSemaphoreCreateBinary();
    // xSemaphore01 = xSemaphoreCreateBinary();

    vInterruptChecker_Ptr = new InterruptChecker();
    scheduler_add_task(vInterruptChecker_Ptr);
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
    scheduler_add_task(new terminalTask(PRIORITY_HIGH));

    /* Consumes very little CPU, but need highest priority to handle mesh network ACKs */
    scheduler_add_task(new wirelessTask(PRIORITY_CRITICAL));

    /* Change "#if 0" to "#if 1" to run period tasks; @see period_callbacks.cpp */
    #if 0
    const bool run_1Khz = false;
    scheduler_add_task(new periodicSchedulerTask(run_1Khz));
    #endif

    scheduler_start(); ///< This shouldn't return
    return -1;
}