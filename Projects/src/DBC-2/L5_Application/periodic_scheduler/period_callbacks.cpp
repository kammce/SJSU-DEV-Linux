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
#include "io.hpp"
#include "periodic_callback.h"
#include "gpio.hpp"
#include "uart2.hpp"
#include "uart3.hpp"
#include "_can_dbc/generated_can.h"
#include "can.h"

/// This is the stack size used for each of the period tasks (1Hz, 10Hz, 100Hz, and 1000Hz)
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

/**
 * This is the stack size of the dispatcher task that triggers the period tasks to run.
 * Minimum 1500 bytes are needed in order to write a debug file if the period tasks overrun.
 * This stack size is also used while calling the period_init() and period_reg_tlm(), and if you use
 * printf inside these functions, you need about 1500 bytes minimum
 */
const uint32_t PERIOD_DISPATCHER_TASK_STACK_SIZE_BYTES = (512 * 3);
const uint32_t baudrate = 100;

void can_BusOffCallback2(uint32_t ibits)
{
    const uint32_t errbit = (ibits >> 16) & 0x1F;
    const char * rxtx = ((ibits >> 21) & 1) ? "RX" : "TX";
    const uint32_t errc = (ibits >> 22) & 3;

    printf("\n\n ***** CAN BUS ENTERED ERROR STATE!\n");
    printf("ERRC = %#x, ERRBIT = %#x while %s\n", (unsigned int)errc, (unsigned int)errbit, rxtx);
}


/// Called once before the RTOS is started, this is a good place to initialize things once
bool period_init(void)
{
	bool ok = CAN_init(can2, baudrate, 10, 10, can_BusOffCallback2, NULL);
    printf("CAN init: %s\n", ok ? "OK" : "ERROR");
	//CAN_bypass_filter_accept_all_msgs();
    CAN_reset_bus(can2);
    return true; // Must return true upon success
}

/// Register any telemetry variables
bool period_reg_tlm(void)
{
    // Make sure "SYS_CFG_ENABLE_TLM" is enabled at sys_config.h to use Telemetry
    return true; // Must return true upon success
}

/**
 * Below are your periodic functions.
 * The argument 'count' is the number of times each periodic task is called.
 */
void period_1Hz(uint32_t count)
{
	if(CAN_is_bus_off(can2))
	{
		printf("CAN BUS OFF!\nRESTARTING!!");
		CAN_reset_bus(can2);
	}
	LE.toggle(1);
}

void period_10Hz(uint32_t count)
{
	// Send out Motor command at 10Hz
    MOTOR_STATUS_t motor_status = { 0 };
    motor_status.MOTOR_STATUS_wheel_error = 0;
    motor_status.MOTOR_STATUS_speed_kph = count;

    can_msg_t can_msg = { 0 };

    // Encode the CAN message's data bytes, get its header and set the CAN message's DLC and length
    dbc_msg_hdr_t msg_hdr = dbc_encode_MOTOR_STATUS(can_msg.data.bytes, &motor_status);
    can_msg.msg_id = msg_hdr.mid;
    can_msg.frame_fields.data_len = msg_hdr.dlc;

    // Queue the CAN message to be sent out
    CAN_tx(can2, &can_msg, 0);
    //LE.toggle(2);
}

void period_100Hz(uint32_t count)
{
    //LE.toggle(3);
}

// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{
    //LE.toggle(4);
}
