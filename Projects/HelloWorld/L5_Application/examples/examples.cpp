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
#include "file_logger.hpp"
#include "io.hpp"
#include "c_tlm_var.h"
#include "io.hpp"
#include "wireless.h"



example_task::example_task() :
    scheduler_task("ex_simple", 4*1024, PRIORITY_HIGH)
{
    /**
     * This is optional such that run() will be called every 3000ms.
     * If you don't do this, run() will be called without a delay.
     */
    setRunDuration(3000);
}

bool example_task::run(void *p)
{
    printf("\n---------------------------------\n"
           "Status Report: \n");
    printf("Temperature: %i F\n", (int)TS.getFarenheit());
    printf("CPU Usage : %i %%\n", getInstantCpuUsage());    /* get OUR tasks' cpu usage */
    printf("Free stack: %i bytes\n", (int)getFreeStack());  /* get number of bytes of free stack of our task */

    for(volatile int i=0; i<0xfffff; i++) {
        ; // Do dummy work
    }

    return true;
}





example_io_demo::example_io_demo() :
        scheduler_task("ex_demo", 4*1024, PRIORITY_LOW)
{
    setRunDuration(100);
}

bool example_io_demo::run(void *p)
{
    static uint8_t pkt_counter = 0;
    const uint8_t switches = SW.getSwitchValues();
    mesh_packet_t pkt;

    enum {
        sw1 = (1 << 0),
        sw2 = (1 << 1),
        sw3 = (1 << 2),
        sw4 = (1 << 3),
    };

    switch(switches)
    {
        case sw1 :
            printf("Acceleration: %4d %4d %4d\n", AS.getX(), AS.getY(), AS.getZ());
            break;

        case sw2 :
            printf("Light : %4d\n", LS.getRawValue());
            break;

        case sw3 :
            printf("Temperature: %i\n", (int) TS.getFarenheit());
            break;

        case sw4 :
            /* Send broadcast message, and increment led number if we get a packet back */
            if (!wireless_send(MESH_BROADCAST_ADDR, mesh_pkt_nack, (char*)&pkt_counter, 1, 0)) {
                puts("Failed to send packet");
            }
            else if (wireless_get_rx_pkt(&pkt, 500)) {
                LD.setNumber(pkt_counter++);
            }
            else {
                puts("Broadcast message not received!");
            }
            break;

        default :
            break;
    }

    /* Only set LEDs when one of the switches are pressed, or LEDs are ON
     * If LEDs are on, this will turn them off.
     */
    if(switches || 0 != LE.getValues()) {
        LE.setAll(switches);
    }

    /* If we receive a broadcast message with 1 byte, send something back */
    if (wireless_get_rx_pkt(&pkt, 0) && 1 == pkt.info.data_len) {
        wireless_send(MESH_BROADCAST_ADDR, mesh_pkt_nack, "ack", 3, 0);
        pkt_counter = pkt.data[0];
        LD.setNumber(pkt_counter);
    }

    return true;
}





/**
 * This example shows how to setup alarms in the form of semaphore
 * that can be given every second, minute, hour etc.
 */
example_alarm::example_alarm() :
    scheduler_task("ex_alarm", 4*1024, PRIORITY_LOW),
    mAlarmSec(NULL),
    mAlarmMin(NULL)
{

}

bool example_alarm::init(void)
{
    vSemaphoreCreateBinary(mAlarmSec);
    vSemaphoreCreateBinary(mAlarmMin);

    return (mAlarmSec != NULL && mAlarmMin != NULL);
}

bool example_alarm::taskEntry(void)
{
    rtc_alarm_create_recurring(everySecond, &mAlarmSec);
    rtc_alarm_create_recurring(everyMinute, &mAlarmMin);

    return true;
}

bool example_alarm::run(void *p)
{
    static int print_first_sixty_secs = 0;
    ++print_first_sixty_secs;

    /**
     * Note that we use MAX delay while checking for mAlarmSec because if a second
     * hasn't elapsed, then by definition, minute and night won't elapse.
     */
    if (xSemaphoreTake(mAlarmSec, portMAX_DELAY)) {
        if (print_first_sixty_secs <= 60) {
            printf("tick %i -- ", (int)rtc_gettime().sec);
        }
    }
    if (xSemaphoreTake(mAlarmMin, 0)) {
        puts("A minute has elapsed!");
    }

    return true;
}






/**
 * Queueset example shows how to use FreeRTOS to block on MULTIPLE semaphores or queues.
 * The idea is that we want to call our run() when EITHER of mSec, or mMin semaphore
 * is ready.
 * This example also shows how to log information to "log" file on flash memory.
 */
example_logger_qset::example_logger_qset() :
    scheduler_task("ex_log_qset", 2*1024, PRIORITY_LOW),
    mSec(NULL),
    mMin(NULL)
{
    vSemaphoreCreateBinary(mSec);
    vSemaphoreCreateBinary(mMin);
}

bool example_logger_qset::init(void)
{
    rtc_alarm_create_recurring(everySecond, &mSec);
    rtc_alarm_create_recurring(everyMinute, &mMin);

    /**
     * We want to block on both seconds and minutes, but any queues or semaphores
     * must be empty before adding to the queue set.  Binary sempaphore is not
     * empty after creating, so perform xSemaphoreTake()
     */
    xSemaphoreTake(mSec, 0);
    xSemaphoreTake(mMin, 0);
    initQueueSet(2, 2, mSec, mMin);

    /**
     * Queue Set will use this block time, and only call run() if either
     * mSec or mMin are given or when this timeout occurs.
     */
    setQueueSetBlockTime(portMAX_DELAY);

    return (mSec != NULL && mMin != NULL);
}

bool example_logger_qset::run(void *p)
{
    static int count = 0;

    /**
     * Let this task run, then see the data logged at the log file :
     * Use this terminal command: "cat log"
     */

    if (getQueueSetSelection() == mSec) {
        // This must work but just in case, return "false" to stop this task.
        if(!xSemaphoreTake(mSec, 0)) {
            return false;
        }

        LOG_INFO_PRINTF("Example log info");
    }
    else if(getQueueSetSelection() == mMin) {
        if(!xSemaphoreTake(mMin, 0)) {
            return false;
        }
        LOG_WARN_PRINTF("Example log warning");
    }
    else {
        LOG_ERROR("I should not get here!");
        return false;
    }

    /** When count exceeds 60, we stop this task on purpose */
    if (count++ < 60) {
        vTaskSuspend(getTaskHandle());
    }
    else {
        /**
         * Information is flushed automatically when file logger's buffer gets full,
         * but manually flush everything we've got.
         */
        FileLogger::getInstance().flush();
        puts("Information was logged to a file.  Type 'cat log' to see it.");
        puts("Suspending this task on purpose");
        return false;
    }

    return false;
}






/**
 * This example shows how you can save your variables to flash memory such that
 * when the program or the board reboots, it will retrieve previous values.
 */
example_nv_vars::example_nv_vars() :
    scheduler_task("nv_vars", 1*1024, PRIORITY_LOW)
{
    mVarWeDontWantToLose = 0;
}
bool example_nv_vars::regTlm(void)
{
    #if ENABLE_TELEMETRY
        // Get the "disk" component list, this list is saved on disk periodically when variables change value
        tlm_component *disk = tlm_component_get_by_name(DISK_TLM_NAME);

        // Register the variable as "disk variable" whose value will be saved
        TLM_REG_VAR(disk, mVarWeDontWantToLose, tlm_int);
    #endif

    // Notice that after about 2 min of no terminal command, a message will print something like :
    // "disk variables changed, changes saved to disk"
    // Even if you reboot the board, this variable will not start from 0, but from the value
    // that was saved to the disk.
    return true;
}
bool example_nv_vars::run(void *p)
{
    // Change the variable every 60 seconds :
    mVarWeDontWantToLose++;
    vTaskDelay(60 * 1000);

    return true;
}






/**
 * This example shows how two tasks can exchange information with each other using a queue.
 *      addSharedObject() adds a pointer to be shared, which we use to share queue handle.
 *      getSharedObject() gets the pointer which all tasks can obtain.
 */
queue_tx::queue_tx() :
    scheduler_task("ex_qtx", 1*1024, PRIORITY_LOW)
{
    /* Nothing to do */
}
bool queue_tx::init(void)
{
    // call our run() every second :
    setRunDuration(1000);

    /**
     * Create a queue, and add to mShared object by the name of "my_queue"
     * One small error is not to check for NULL from xQueueCreate()
     */
    xQueueHandle my_queue = xQueueCreate(1, sizeof(int));
    return addSharedObject("my_queue", my_queue);
}
bool queue_tx::run(void *p)
{
    // Send only 10 samples, then suspend both tasks.
    static int sample = 0;
    if (++sample > 10) {
        // Get the pointer of the "ex_qrx" task
        scheduler_task *qrx_task = getTaskPtrByName("ex_qrx");

        // Suspend the other task and ourselves
        // It's worth checking for qrx_task being NULL though ;)
        qrx_task->suspend();
        suspend();
    }

    /**
     * Send to "my_queue" our integer
     * One small error is that we don't check for NULL pointer from getSharedObject()
     */
    xQueueHandle queue_handle = getSharedObject("my_queue");
    return xQueueSend(queue_handle, &sample, 2000);
}

/**
 * Simple task that is receiving data from queue_tx class.
 * Since queue is already created by queue_tx::init(), we just use it in our run().
 */
queue_rx::queue_rx() :
    scheduler_task("ex_qrx", 2*1024, PRIORITY_LOW)
{

}
bool queue_rx::run(void *p)
{
    int sample = 0;
    xQueueHandle queue_handle = getSharedObject("my_queue");

    if (xQueueReceive(queue_handle, &sample, portMAX_DELAY)) {
        printf("Got %i from my_queue\n", sample);
    }
    return true;
}
