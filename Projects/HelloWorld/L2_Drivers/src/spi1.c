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

#include "spi1.h"
#include "sys_config.h"



/**
 * Initializes registers for SSP DMA
 */
void spiDmaInitialize();



void spi1_set_max_clock(unsigned int max_clock_mhz)
{
    unsigned int divider = 2;
    const unsigned int cpuClockMhz = sys_get_cpu_clock() / (1000 * 1000UL);

    // Keep scaling down divider until calculated is higher
    // Example:
    // 60, need 18
    // 60/2   18 < 30 YES
    // 60/4   18 < 15 NO
    while(max_clock_mhz < (cpuClockMhz / divider) && divider <= 254)
    {
        divider += 2;
    }

    LPC_SSP1->CPSR = divider;
}

void spi1_init(void)
{
    LPC_SC->PCONP |= (1 << 10);     // SPI1 Power Enable
    LPC_SC->PCLKSEL0 &= ~(3 << 20); // Clear clock Bits
    LPC_SC->PCLKSEL0 |=  (1 << 20); // CLK / 1

    // Pins are initialized by bio.h */

    LPC_SSP1->CR0 = 7;          // 8-bit mode
    LPC_SSP1->CR1 = (1 << 1);   // Enable SSP as Master

    spi1_set_max_clock(12);
    spiDmaInitialize();
}
