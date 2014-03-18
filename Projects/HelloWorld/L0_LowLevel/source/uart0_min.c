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

#include "LPC17xx.h"
#include "sys_config.h"



void uart0_init(unsigned int baud_rate)
{
    const uint16_t divider = (sys_get_cpu_clock() / (16 * baud_rate) + 0.5);

	LPC_SC->PCONP |= (1 << 3);          // Enable power to UART0
	LPC_SC->PCLKSEL0 &= ~(3 << 6);
	LPC_SC->PCLKSEL0 |=  (1 << 6);      // Uart clock = CPU / 1

	LPC_PINCON->PINSEL0 &= ~(0xF << 4); // Clear values
	LPC_PINCON->PINSEL0 |= (0x5 << 4);  // Set values for UART0 Rx/Tx

	LPC_UART0->LCR = (1 << 7);	// Enable DLAB
	LPC_UART0->DLM = (divider >> 8);
	LPC_UART0->DLL = (divider >> 0);
	LPC_UART0->LCR = 3;			// 8-bit data
}

char uart0_getchar(char notused)
{
    while(! (LPC_UART0->LSR & (1 << 0)))
    {
        ;
    }
    return LPC_UART0->RBR;
}

char uart0_putchar(char out)
{
	//while(! (LPC_UART0->LSR & (1 << 6)));
	LPC_UART0->THR = out;
    while(! (LPC_UART0->LSR & (1 << 6)));
	return 1;
}

void uart0_puts(const char* c_string)
{
	char* p = (char*) c_string;
	while(*p)
	{
		uart0_putchar(*p);
		p++;
	}
	uart0_putchar('\n');
}
