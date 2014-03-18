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
 * @ingroup Drivers
 */
#ifndef SPI0_H__
#define SPI0_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "LPC17xx.h"
#include "sys_config.h"



/**
 * Sets SPI Clock speed
 * @param maxClockMhz   The maximum speed of this SPI in Megahertz
 * @note The speed may be set lower to maxClockMhz if it cannot be attained.
 */
inline void spi0_set_max_clock(unsigned int max_clock_mhz)
{
    unsigned int divider = 2;
    const unsigned int cpuClockMhz = sys_get_cpu_clock() / (1000 * 1000UL);

    while(max_clock_mhz < (cpuClockMhz / divider) && divider <= 254)
    {
        divider += 2;
    }

    LPC_SSP0->CPSR = divider;
}

/**
 * Initializes SPI 1
 * Configures CLK, MISO, MOSI pins with a slow SCK speed
 */
inline void spi0_init(unsigned int max_clock_mhz)
{
    LPC_SC->PCONP |= (1 << 20);     // SPI0 Power Enable
    LPC_SC->PCLKSEL1 &= ~(3 << 10); // Clear clock Bits
    LPC_SC->PCLKSEL1 |=  (1 << 10); // CLK / 1

    // Pins are initialized by bio.h */
    LPC_SSP0->CR0 = 7;          // 8-bit mode
    LPC_SSP0->CR1 = (1 << 1);   // Enable SSP as Master

    spi0_set_max_clock(max_clock_mhz);
}

/**
 * Exchanges a byte over SPI bus
 * @param out   The byte to send out
 * @returns     The byte received over SPI
 */
inline char spi0_exchange_byte(char out)
{
    LPC_SSP0->DR = out;
    while(LPC_SSP0->SR & (1 << 4)); // Wait until SSP is busy
    return LPC_SSP0->DR;
}



#ifdef __cplusplus
}
#endif
#endif /* SPI0_H__ */
