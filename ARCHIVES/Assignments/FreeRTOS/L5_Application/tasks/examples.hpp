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
 * @brief Contains FreeRTOS task examples.
 */

#ifndef TASKS_HPP_
#define TASKS_HPP_

#include "scheduler_task.hpp"
#include "shared_handles.h"
#include "uart3.hpp"
#include "rn_xv_task.hpp"
#include "FreeRTOS.h"
#include "semphr.h"

/**
 * This example shows how to use FreeRTOS based alarms such that you can design
 * tasks that do work every second, every minute etc.
 */
class example_alarm : public scheduler_task
{
    public:
        example_alarm();
        bool init(void);        ///< We create our semaphores heres. No FreeRTOS Blocking API here!!!
        bool taskEntry(void);   ///< We enable the alarms here.
        bool run(void *p);      ///< We run our code here

    private:
        SemaphoreHandle_t mAlarmSec;
};

#endif /* TASKS_HPP_ */
