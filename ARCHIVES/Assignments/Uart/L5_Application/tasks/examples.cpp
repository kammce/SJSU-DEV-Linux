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

#include "examples.hpp"
#include "rtc_alarm.h"
#include "rtc.h"
#include "file_logger.h"
#include "io.hpp"
#include "c_tlm_var.h"
#include "io.hpp"
#include "wireless.h"
#include "uart0.hpp"

/**
 * This example shows how to setup alarms in the form of semaphore
 * that can be given every second, minute, hour etc.
 */
// example_alarm::example_alarm() :
//     scheduler_task("ex_alarm", 4 * 512, PRIORITY_LOW),
//     mAlarmSec(NULL)
// {

// }

// bool example_alarm::init(void)
// {
//     mAlarmSec = xSemaphoreCreateBinary();

//     return (mAlarmSec != NULL);
// }

// bool example_alarm::taskEntry(void)
// {
//     rtc_alarm_create_recurring(everySecond, &mAlarmSec);
//     return true;
// }

// bool example_alarm::run(void *p)
// {
//     static int print_first_sixty_secs = 0;
//     ++print_first_sixty_secs;

//     /**
//      * Note that we use MAX delay while checking for mAlarmSec because if a second
//      * hasn't elapsed, then by definition, minute won't elapse.
//      */
//     if (xSemaphoreTake(mAlarmSec, portMAX_DELAY)) {
//         if (print_first_sixty_secs <= 60) {
//             printf("tick %i -- ", (int)rtc_gettime().sec);
//         }
//     }

//     return true;
// }