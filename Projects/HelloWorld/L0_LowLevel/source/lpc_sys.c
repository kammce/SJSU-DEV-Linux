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

#include "lpc_sys.h"
#include "sys_config.h"



static void_func_t m_rit_callback = 0;                  /**< RIT Callback function pointer */


/** RIT Interrupt function */
void RIT_IRQHandler()
{
    m_rit_callback();
    LPC_RIT->RICTRL |= 1;
}

void sys_get_mem_info_str(char* pBuff)
{
    sys_mem_t info = sys_get_mem_info();
    sprintf(pBuff,
            "Memory Information:\n"
            "Global Used   : %5d\n"
            "Heap   Used   : %5d\n"
            "Heap Avail.   : %5d\n"
            "System Avail. : %5d\n",
            info.used_global, info.used_heap, info.avail_heap, info.avail_sys);
}

void sys_rit_setup(void_func_t function, unsigned int timeMs)
{
    if (0 == function) {
        return;
    }
    // Divide by zero guard
    if(0 == timeMs) {
        timeMs = 1;
    }

    // Power up first otherwise writing to RIT will give us Hard Fault
    LPC_SC->PCONP |= (1 << 16);

    // Enable CLK/1 to simplify RICOMPVAL calculation below
    LPC_SC->PCLKSEL1 &= ~(3 << 26);
    LPC_SC->PCLKSEL1 |=  (1 << 26);

    LPC_RIT->RICTRL = 0;
    LPC_RIT->RICOUNTER = 0;
    LPC_RIT->RIMASK = 0;
    LPC_RIT->RICOMPVAL = sys_get_cpu_clock() / (1000 / timeMs);

    // Clear timer upon match, and enable timer
    LPC_RIT->RICTRL = (1 << 0) | (1 << 1) | (1 << 3);

    // Enable System Interrupt and connect the callback
    m_rit_callback = function;
    NVIC_EnableIRQ(RIT_IRQn);
}

void sys_rit_disable(void)
{
    LPC_RIT->RICTRL = 0;
    NVIC_DisableIRQ(RIT_IRQn);
}

bool sys_rit_running(void)
{
    return !!(LPC_RIT->RICTRL & (1 << 3));
}
