#include <stdio.h>
#include "tasks.hpp"
#include "examples/examples.hpp"
#include "io.hpp"
#include "eint.h"
#include "periodic_scheduler/periodic_callback.h"
#include "uart2.hpp"
#include "uart3.hpp"
#include "utilities.h"

int main(void)
{
    scheduler_add_task(new terminalTask(PRIORITY_HIGH));
    scheduler_add_task(new wirelessTask(PRIORITY_CRITICAL));
    /* Change "#if 0" to "#if 1" to run period tasks; @see period_callbacks.cpp */

    Uart2& u2 = Uart2::getInstance();
    u2.init(38400);

    Uart3& u3 = Uart3::getInstance();
    u3.init(38400);

    u2.flush();
    u3.flush();

    #if 1
    scheduler_add_task(new periodicSchedulerTask());
    scheduler_add_task(new ComTask());
    scheduler_add_task(new ComTask2());
    #endif

    scheduler_start(); ///< This shouldn't return
    return -1;
}
