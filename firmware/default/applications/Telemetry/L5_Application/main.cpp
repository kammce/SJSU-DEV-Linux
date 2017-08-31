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
 * @brief This is the application entry point.
 * 			FreeRTOS and stdio printf is pre-configured to use uart0_min.h before main() enters.
 * 			@see L0_LowLevel/lpc_sys.h if you wish to override printf/scanf functions.
 *
 */
#include <stdint.h>

#include "tasks.hpp"
#include "examples/examples.hpp"
#include "periodic_scheduler/periodic_callback.h"
#include "uart2.hpp"
#include "uart3.hpp"
#include "utilities.h"
#include "tlm/c_tlm_var.h"
#include "io.hpp"

class vChangerTask : public scheduler_task
{
    public:
        vChangerTask(uint8_t priority) : scheduler_task("vChangerTask", 2048, priority) {}
        uint32_t x;
        float y;
        int32_t z;
        uint32_t inc;
        bool init(void)
        {
            // Clear variables
            x = 0;
            y = 0;
            z = 0;
            inc = 1;
            // Get pointer to telemetry bucket
            tlm_component * bucket = tlm_component_add("App");
            // Add variables to bucket
            TLM_REG_VAR(bucket, x, tlm_uint);
            TLM_REG_VAR(bucket, y, tlm_float);
            TLM_REG_VAR(bucket, z, tlm_int);
            return true;
        }
        bool run(void *p)
        {
            x++;
            y += 0.1;

            inc = (z < 0 || z > 99) ? -inc : inc;
            z += inc;

            LD.setNumber(z);

            vTaskDelay(100);
            return true;
        }
};

int main(void)
{
    /**
     * A few basic tasks for this bare-bone system :
     *      1.  Terminal task provides gateway to interact with the board through UART terminal.
     *      2.  Remote task allows you to use remote control to interact with the board.
     *      3.  Wireless task responsible to receive, retry, and handle mesh network.
     *
     * Disable remote task if you are not using it.  Also, it needs SYS_CFG_ENABLE_TLM
     * such that it can save remote control codes to non-volatile memory.  IR remote
     * control codes can be learned by typing the "learn" terminal command.
     */
    scheduler_add_task(new terminalTask(PRIORITY_HIGH));
    /* Consumes very little CPU, but need highest priority to handle mesh network ACKs */
    scheduler_add_task(new wirelessTask(PRIORITY_CRITICAL));
    scheduler_add_task(new vChangerTask(PRIORITY_LOW));
    scheduler_start(); ///< This shouldn't return
    return -1;
}
