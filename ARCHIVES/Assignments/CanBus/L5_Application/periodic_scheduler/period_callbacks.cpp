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
 * This contains the period callback functions for the periodic scheduler
 *
 * @warning
 * These callbacks should be used for hard real-time system, and the priority of these
 * tasks are above everything else in the system (above the PRIORITY_CRITICAL).
 * The period functions SHOULD NEVER block and SHOULD NEVER run over their time slot.
 * For example, the 1000Hz take slot runs periodically every 1ms, and whatever you
 * do must be completed within 1ms.  Running over the time slot will reset the system.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "io.hpp"
#include "periodic_callback.h"
#include "gpio.hpp"
#include "uart2.hpp"
#include "uart3.hpp"
#include "../../L2_Drivers/can.h"

edge_t Signal_E = RISE;

/// This is the stack size used for each of the period tasks (1Hz, 10Hz, 100Hz, and 1000Hz)
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

/**
 * This is the stack size of the dispatcher task that triggers the period tasks to run.
 * Minimum 1500 bytes are needed in order to write a debug file if the period tasks overrun.
 * This stack size is also used while calling the period_init() and period_reg_tlm(), and if you use
 * printf inside these functions, you need about 1500 bytes minimum
 */
const uint32_t PERIOD_DISPATCHER_TASK_STACK_SIZE_BYTES = (512 * 3);

can_t can = can2;

void can_BusOffCallback2(uint32_t ibits)
{
    const uint32_t errbit = (ibits >> 16) & 0x1F;
    const char * rxtx = ((ibits >> 21) & 1) ? "RX" : "TX";
    const uint32_t errc = (ibits >> 22) & 3;

    printf("\n\n ***** CAN BUS ENTERED ERROR STATE!\n");
    printf("ERRC = %#x, ERRBIT = %#x while %s\n", errc, errbit, rxtx);
}

const uint32_t baudrate = 100;

/// Called once before the RTOS is started, this is a good place to initialize things once
bool period_init(void)
{
	bool ok = CAN_init(can, baudrate, 10, 10, can_BusOffCallback2, NULL);
    printf("CAN init: %s\n", ok ? "OK" : "ERROR");
	//CAN_bypass_filter_accept_all_msgs();
    CAN_reset_bus(can);
    return true; // Must return true upon success
}

/// Register any telemetry variables
bool period_reg_tlm(void)
{
    // Make sure "SYS_CFG_ENABLE_TLM" is enabled at sys_config.h to use Telemetry
    return true; // Must return true upon success
}

void period_1Hz(void) {
	if(CAN_is_bus_off(can))
	{
		printf("CAN BUS OFF!\nRESTARTING!!");
		CAN_reset_bus(can);
	}
}
void period_10Hz(void) {}
void period_100Hz(void) {
    static can_msg_t msg;
	if(SW.getSwitch(3))
	{
	    msg.msg_id = 0x123;
	    msg.frame_fields.is_29bit = 0;
	    msg.frame_fields.data_len = 1;
	    msg.data.qword = 0xAA;
	    CAN_tx(can, &msg, 1);
	    printf("0xAA Message Send Complete\n");
	}
	else
	{
	    msg.msg_id = 0x123;
	    msg.frame_fields.is_29bit = 0;
	    msg.frame_fields.data_len = 1;
	    msg.data.qword = 0x00;
	    CAN_tx(can, &msg, 1);
	    //printf("0x00 Message Send Complete\n");
	}
}
void period_1000Hz(void) {}
