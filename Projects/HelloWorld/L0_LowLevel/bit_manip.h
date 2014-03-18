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
 * @brief Provides bit-manipulation macros
 */
#ifndef BIT_MANIP_H__
#define BIT_MANIP_H__
#ifdef __cplusplus
extern "C" {
#endif



/**
 * One bit manipulation macro.
 * Usage:
 * @code
 *      BIT(LPC_GPIO1->FIODIR).b5 = 1;
 *      BIT(my_var).b0 = 1;
 * @endcode
 */
#define BIT(reg)  (*((volatile OneBitStructType*)&(reg)))




/**
 * 1-bit structure
 * Use BIT() macro for bit manipulation of your memory/variable.
 */
typedef union
{
    // 32-bit structure below overlaps with this 32-bit integer
    // because each var of a union uses same base memory location.
    unsigned int full32bit;

    // : 1 (colon 1) means use only 1 bit size for the variable.
    struct {
        unsigned int b0  :1; unsigned int b1  :1; unsigned int b2  :1; unsigned int b3  :1;
        unsigned int b4  :1; unsigned int b5  :1; unsigned int b6  :1; unsigned int b7  :1;
        unsigned int b8  :1; unsigned int b9  :1; unsigned int b10 :1; unsigned int b11 :1;
        unsigned int b12 :1; unsigned int b13 :1; unsigned int b14 :1; unsigned int b15 :1;
        unsigned int b16 :1; unsigned int b17 :1; unsigned int b18 :1; unsigned int b19 :1;
        unsigned int b20 :1; unsigned int b21 :1; unsigned int b22 :1; unsigned int b23 :1;
        unsigned int b24 :1; unsigned int b25 :1; unsigned int b26 :1; unsigned int b27 :1;
        unsigned int b28 :1; unsigned int b29 :1; unsigned int b30 :1; unsigned int b31 :1;
    }__attribute__((packed));
    // packed means pack all 1 bit members tightly

} OneBitStructType;



#ifdef __cplusplus
}
#endif
#endif /* BIT_MANIP_H__ */
