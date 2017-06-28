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

/* Globals */
GPIO *gpio_cap;
GPIO *expander_if[MAX_IF] = {NULL};
LPC_TIM_TypeDef *cap_timer_ptr = NULL;

pin_def pin_init_list[] = {
    {IF_SEL0, P2_0, LVL_LOW,  DIR_OUT},   /* Output, active high, to '251 A / '259 A */
    {IF_SEL1, P2_1, LVL_LOW,  DIR_OUT},   /* Output, active high, to '251 B / '259 B */
    {IF_SEL2, P2_2, LVL_LOW,  DIR_OUT},   /* Output, active high, to '251 C / '269 C */
    {IF_DIN,  P2_3, LVL_LOW,  DIR_OUT},   /* Output, active high, to '259 Din */
    {IF_LE,   P2_4, LVL_HIGH, DIR_OUT},   /* Output, active low,  to '259 LE# */
    {IF_DOUT, P2_5, LVL_LOW,  DIR_IN},    /* Input, active low, from '251 Y#*/
 //   {IF_TEST, P2_6, LVL_LOW,  DIR_IN},    /* Input, active high, from '251 Y*/
    {MAX_IF}
};



void expander_init(int irq_pin, void (*irq_callback)(void))
{

    /* Set all latches low */
    for(int i = 0; i < 8; i++)
    {

    }

    /* Build expander interface */
    for(int i = 0; pin_init_list[i].id != MAX_IF; i++)
    {
        /* Get record associated with this pin */
        pin_def *def = &pin_init_list[i];

        /* Allocate GPIO */
        GPIO *pin = new GPIO(def->type);

        /* Map this GPIO to expander interface */
        expander_if[def->id] = pin;

        /* Set output level */
        pin->set(def->reset_level);

        /* Set pin direction */
        if(def->direction)
        {
            /* Output */
            pin->setAsOutput();
            pin->disablePullUpPullDown();
        }
        else
        {
            /* Input */
            pin->setAsInput();
            pin->enablePullUp();
        }
    }

    /* Configure P1.19 as GPIO input with pull-up enabled */
    gpio_cap = new GPIO(P1_19);
    gpio_cap->setAsInput();
    gpio_cap->enablePullUp();

    /* Select PCLK/1 for for TIMER1 peripheral */
    PCLKSEL0 &= ~(0x03 << 4);
    PCLKSEL0 |=  (0x01 << 4);

    /* Configure CAP1.1 function on P1.19 */
    PINSEL3  &= ~(0x03 << 6);
    PINSEL3  |=  (0x03 << 6); // Select CAP1.1 on P1.19

    /* Configure Timer #1 */
    const lpc_timer_t sys_timer_source = (lpc_timer_t)1;

    // Get the IRQ number of the timer to enable the interrupt
    const IRQn_Type timer_irq = lpc_timer_get_irq_num(sys_timer_source);

    // Initialize the timer structure pointer
    cap_timer_ptr = lpc_timer_get_struct(sys_timer_source);

    /* Enable timer 1 and set running at 1us */
    lpc_timer_enable(sys_timer_source, 1);

    /* Disable all match and capture events */
    cap_timer_ptr->MCR = 0;
    cap_timer_ptr->CCR = 0;

    /* Enable loading CR1 on either edge (RE,FE) of CAP1.1 */
    cap_timer_ptr->CCR |= (1 << 3); // Set CAP1RE so that 0->1 on CAP1.1 will load CR1 with TC
    cap_timer_ptr->CCR |= (1 << 4); // Set CAP1FE so that 1->0 on CAP1.1 will load CR1 with TC

    /* Enable interrupt for capture channel 1 event */
    cap_timer_ptr->CCR |= (1 << 5);
    cap_timer_ptr->IR  |= (1 << 5);

    /* Counter ticks at 48 MHz/(PR+1) = 48 MHz (20.82ns/tick) */
    cap_timer_ptr->PR = 0;

    /* Install capture event ISR */
    isr_register(timer_irq, irq_callback);

    /* Enable timer #1 interrupt processing through NVIC */
    NVIC_EnableIRQ(timer_irq);
    NVIC_SetPriority(timer_irq, IP_default);
}


/* Reset all output pins to their default state */
void expander_reset(void)
{
    /* Set output level for all pins in interface */
    for(int i = 0; pin_init_list[i].id != MAX_IF; i++)
        expander_if[pin_init_list[i].id]->set(pin_init_list[i].reset_level);
}



/* Set latch address or multiplexer input channel */
void expander_set_address(int which)
{
    /* Disable capture to prevent false transitions when the multiplexer input changes */
    cap_timer_ptr->CCR &= ~CCR_CAP1_EDGES;

    /* Select register address */
    expander_if[IF_SEL0]->set((which >> 0) & 1);
    expander_if[IF_SEL1]->set((which >> 1) & 1);
    expander_if[IF_SEL2]->set((which >> 2) & 1);

    /* Wait for address to settle */
    delay_us(EXIF_ADDR_SETTLE);

    /* Re-enable capture */
    cap_timer_ptr->CCR |= CCR_CAP1_EDGES;
}

/* Load addressable latch with a data bit */
void expander_write(int which, int state)
{
    /* Assign data to load into register */
    expander_if[IF_DIN]->set(state);

    /* Select register address */
    expander_set_address(which);

    /* Pulse register load strobe low for 1us */
    expander_if[IF_LE]->set(false);
    delay_us(EXIF_DATA_LATCH_PULSE_WIDTH);
    expander_if[IF_LE]->set(true);
}

/* Read multiplexer output */
bool expander_read(int which)
{
    /* Select multiplexer channel */
    expander_set_address(which);

    /* Read data output */
    return expander_if[IF_DOUT]->read();
}


/* End */
