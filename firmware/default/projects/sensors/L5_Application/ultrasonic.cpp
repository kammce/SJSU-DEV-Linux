/*
 
 */

#include <stdint.h>
#include <stdio.h>
#include "io.hpp"
#include "imu_sensors.hpp"
#include <math.h>
#include "_can_dbc/generated_can.h"
#include "can.h"
#include "gpio.hpp"
#include "lpc_timers.h"
#include "expander.hpp"
#include "ultrasonic.hpp"
#include "eint.h"

extern LPC_TIM_TypeDef *cap_timer_ptr;
extern GPIO *gpio_cap;
static bool capture_enabled = false;
sensor_response *current_response;


void ultrasonic_init(void)
{
    /* */
}

void ping_ultrasonic_sensors(int timeout, sensor_response *response)
{
    capture_enabled = true;
    current_response = response;

    for(int i = 0; i < MAX_SENSORS; i++)
    {
        current_response = &response[i];

        /* Assume normal result state */
        current_response->type = US_RESULT_NORMAL;

        /* Trigger sensor */
        expander_write(i, US_TRIG_LEVEL_ON);
        current_response->state = ST_REQUEST;
        delay_us(US_TRIG_PULSE_WIDTH);
        expander_write(i, US_TRIG_LEVEL_OFF);

        /* Wait until either of:
         * 1. 50ms have elapsed.
         * 2. An falling-edge interrupt has occurred (ranging is complete).
         */
        uint32_t ranging_start = T1TC;
        const uint32_t ranging_duration = 2400000;      /* 50ms in 48 MHz clocks */

        while(1)
        {
            uint32_t delta = T1TC - ranging_start;

            /* If 50ms have elapsed, stop waiting */
            if(delta > ranging_duration)
                break;

            /* If a falling edge interrupt advanced to the state machine from M
             * MEASURE to IDLE, we can abort early
             */
            if(current_response->state == ST_IDLE)
                break;
        }

        /* Check if the interrupt occurred */
        if(current_response->state == ST_REQUEST)
        {
            current_response->type = US_RESULT_NORESPONSE;
        }

        /* Post-ranging delay
         * Relates to malformed pulses on sensor 3 (R)
         * Need to investigate this further.
         * Analyzer checks: Unrelated to '259 LE, D, A0-A2
         * Probable cause:
         * - Likely a reflection from the last sensor sendout?
         * - 1-2ms delay doesn't work, 3, 4 seem more stable.
         */
        delay_ms(4); // was 4, then 10
    }
    capture_enabled = false;
}



void echo_capture_isr(void)
{
    const lpc_timer_t sys_timer_source = (lpc_timer_t) 1;
    const IRQn_Type timer_irq = lpc_timer_get_irq_num(sys_timer_source);
    static uint32_t last_capture = 0;

    if(capture_enabled)
    {
        /* Get captured value from timer */
        uint32_t curr_capture = cap_timer_ptr->CR1;

        /* Read pin state to determine interrupt edge */
        bool state = gpio_cap->read();

        switch(current_response->state)
        {
            case ST_IDLE:
                /*
                 * Sensor ranging state is idle and we somehow received
                 * an interrupt spuriously.
                 */
                break;

            case ST_REQUEST:
                /*
                 * Sensor ranging state is waiting for a rising edge
                 */
                if(US_ECHO_ACTIVE(state))
                {
                    /* Got rising edge of echo pulse */
                    current_response->state = ST_MEASURE;
                    last_capture = curr_capture;
                }
                break;

            case ST_MEASURE:
                /*
                 * Sensor ranging state is waiting for a falling edge
                 */
                if(!US_ECHO_ACTIVE(state))
                {
                    /* Got falling edge of echo pulse */
                    /* Measure difference from rising edge capture to falling edge capture */
                    uint32_t duration = curr_capture - last_capture;

                    /* Record duration and advance state machine back to idle */
                    current_response->pulse_width = duration;
                    current_response->state = ST_IDLE;
                }
                break;
        }
    }

    /* Acknowledge timer capture interrupt */
    T1IR |= B_IR_CR1;

    /* Clear pending interrupt through NVIC */
    NVIC_ClearPendingIRQ(timer_irq);
}


/* End */
