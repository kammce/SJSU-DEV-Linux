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
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include "FreeRTOS.h"
#include "semphr.h"

#include "cpp_task.hpp"
#include "utilities.h"
#include "lpc_sys.h"

#include "c_tlm_comp.h"
#include "c_tlm_var.h"
#include "c_tlm_stream.h"



/** @{ Private global variables */
static xTaskHandle mTaskEntryTaskHandle = 0; /**< Task handle that will call taskEntry() for everyone */
static xSemaphoreHandle mRunTask = 0;        /**< Semaphore for a task to proceed to run() method */
static VECTOR<scheduler_task*> *gTaskList;   /**< List of all scheduler_tasks pointers */
/** @} */

VECTOR<scheduler_task::PtrNamePairType> *scheduler_task::mpNamePtrPair;



/** Print two strings followed by newline characters */
static void print_strings(const char *one, const char *two)
{
    while(*one) {
        putchar(*one++);
    }
    puts(two);
}

/**
 * This is the C task that is created multiple times.  Each task receives its
 * own parameter therefore differentiating between different tasks.
 */
void scheduler_c_task_private(void *task_ptr)
{
    scheduler_task& task = *(scheduler_task*)task_ptr;

    /* Have the responsible task call taskEntry() for each task */
    if (mTaskEntryTaskHandle == task.mHandle) {
        bool failure = false;

        print_strings(task.mName, " task calling taskEntry() for all tasks ... ");
        for (uint32_t i=0; i < gTaskList->size(); i++)
        {
            scheduler_task *t = gTaskList->at(i);
            if (!t->taskEntry()) {
                print_strings(t->mName, "  --> FAILED taskEntry()");
                failure = true;
            }
        }

        if (failure) {
            puts("ERROR: Killing FreeRTOS due to error(s)");
            vTaskEndScheduler();
        } else {
            /* Give permission for everyone to start the run() loop */
            for (uint32_t i=0; i < gTaskList->size(); i++)
            {
                xSemaphoreGive(mRunTask);
            }
        }
    }

    // Wait until we're given the go ahead by the task giving semaphore above
    xSemaphoreTake(mRunTask, portMAX_DELAY);

    portTickType xLastWakeTime = xTaskGetTickCount();
    portTickType xNextStatTime = 0;

    for (;;)
    {
        #if (0 != configUSE_QUEUE_SETS)
        if (task.mQueueSet) {
            task.mQueueSetType = xQueueSelectFromSet(task.mQueueSet, task.mQueueSetBlockTime);
        }
        #endif

        // Run the task code and suspend when an error occurs
        if (!task.run((void*)task.mParam)) {
            print_strings(task.mName, " --> FAILURE detected; suspending this task ...");
            vTaskSuspend(0);
        }
        ++(task.mRunCount);

        // Update the task statistics once in a short while :
        if (xTaskGetTickCount() > xNextStatTime) {
            xNextStatTime = xTaskGetTickCount() + OS_MS(task.mStatUpdateRateMs);
            task.mFreeStack = uxTaskGetStackHighWaterMark(task.mHandle);
            task.mCpuPercent = uxTaskGetCpuUsage(task.mHandle);
        }

        // Delay if set
        if (task.mTaskDelayMs) {
            vTaskDelayUntil( &xLastWakeTime, OS_MS(task.mTaskDelayMs));
        }
    }
}

bool scheduler_init_all(bool register_internal_tlm)
{
    bool failure = false;

    /* If not tasks ... */
    if (0 == gTaskList || 0 == gTaskList->size()) {
        puts("ERROR: NO tasks added by scheduler_add_task()");
        failure = true;
        return failure;
    }

    /* Initialize all tasks */
    puts("*  Initializing tasks ...");
    for (uint32_t i=0; i < gTaskList->size(); i++)
    {
        scheduler_task *task = gTaskList->at(i);
        if (!task->init()) {
            print_strings(task->getTaskName(), "  --> FAILED init()");
            failure = true;
        }
    }

    /* Register telemetry for all tasks */
    #if ENABLE_TELEMETRY
    puts("*  Registering tasks' telemetry ...");
    for (uint32_t i=0; i < gTaskList->size(); i++)
    {
        scheduler_task *task = gTaskList->at(i);
        if (!task->regTlm()) {
            failure = true;
        }

        /* Register statistical variables of this task */
        if (register_internal_tlm) {
            tlm_component *comp = tlm_component_add(task->mName);
            if (!tlm_variable_register(comp, "cpu_percent", &(task->mCpuPercent),
                sizeof(task->mCpuPercent), 1, tlm_uint)) {
                failure = true;
            }
            if (!tlm_variable_register(comp, "free_stack", &(task->mFreeStack),
                 sizeof(task->mFreeStack), 1, tlm_uint)) {
                failure = true;
            }
            if (!tlm_variable_register(comp, "run_count", &(task->mRunCount),
                 sizeof(task->mRunCount), 1, tlm_uint)) {
                failure = true;
            }
        }
        if (failure) {
            print_strings(task->mName, "  --> FAILED telemetry registration");
        }
    }

    puts("*  Restoring disk telemetry");
    // Restore telemetry registered by "disk" component
    FILE *fd = fopen(DISK_TLM_NAME, "r");
    if (fd) {
        tlm_stream_decode_file(fd);
        fclose(fd);
    }
    #endif

    puts("*  Creating tasks ...");
    mRunTask = xSemaphoreCreateCounting(gTaskList->size(), 0);
    if (NULL == mRunTask) {
        failure = true;
    }

    for (uint32_t i=0; i < gTaskList->size(); i++)
    {
        scheduler_task *task = gTaskList->at(i);
        if (!xTaskCreate(scheduler_c_task_private,
                        (signed char*)task->mName,      /* Name  */
                        STACK_BYTES(task->mStackSize),  /* Stack */
                        task,                           /* Task param    */
                        task->mPriority,                /* Task priority */
                        &(task->mHandle)))              /* Task Handle   */
        {
            print_strings(task->mName, "  --> FAILED xTaskCreate()");
            failure = true;
        }
    }

    /* Find the task with highest stack, and this task will then be used
     * to call taskEntry() for everyone once FreeRTOS starts.
     * We have one task do taskEntry() for all because otherwise tasks will perform
     * taskEntry() independent of each other, and one task might complete it and
     * start running without other tasks completing their taskEntry().
     */
    uint32_t highestStack = 0;
    for (uint32_t i=0; i < gTaskList->size(); i++)
    {
        scheduler_task *task = gTaskList->at(i);
        if (task->mStackSize > highestStack) {
            highestStack = task->mStackSize;
            mTaskEntryTaskHandle = task->mHandle;
        }
    }

    return failure;
}

void scheduler_add_task(scheduler_task* task)
{
    if (0 == gTaskList) {
        gTaskList = new VECTOR<scheduler_task*>;
    }
    if (NULL != task) {
        gTaskList->push_back(task);
    }
}

void scheduler_start(bool register_internal_tlm)
{
    /* If no failure, start the FreeRTOS scheduler */
    if (!scheduler_init_all(register_internal_tlm)) {
        puts("Starting scheduler ...");
        vTaskStartScheduler();

        // vTaskStartScheduler() should not return
        puts("ERROR: Someone killed the scheduler");
    }
    else {
        puts("ERROR: Refusing to start OS scheduler due to error(s)");
        delay_ms(3000);
        sys_reboot();
    }
}



scheduler_task::scheduler_task(const char *name, uint32_t stack, uint8_t priority, void *param) :
#if (0 != configUSE_QUEUE_SETS)
   mQueueSet(0),
   mQueueSetType(0),
   mQueueSetBlockTime(1000),
#endif
   mHandle(0),
   mFreeStack(0),
   mCpuPercent(0),
   mRunCount(0),
   mTaskDelayMs(0),
   mStatUpdateRateMs(1000),
   mName(name),
   mParam(param),
   mPriority(priority),
   mStackSize(stack)
{

}

uint8_t scheduler_task::getInstantCpuUsage(void) const
{
    return uxTaskGetCpuUsage(getTaskHandle());
}

uint8_t scheduler_task::getTotalSysCpuPercentage(void) const
{
    return (100 - getCpuIdlePercentage());
}

uint8_t scheduler_task::getCpuIdlePercentage(void) const
{
    return uxTaskGetCpuUsage(xTaskGetIdleTaskHandle());
}

scheduler_task* scheduler_task::getTaskPtrByName(const char *name)
{
    for(unsigned int i=0; i < gTaskList->size(); i++) {
        scheduler_task *task = gTaskList->at(i);
        if (0 == strcmp(name, task->getTaskName())) {
            return task;
        }
    }
    return NULL;
}

#if (0 != configUSE_QUEUE_SETS)
void scheduler_task::initQueueSet(uint32_t queueSetSize, uint32_t count, ...)
{
    void *handle = 0;
    va_list vl;
    va_start(vl, count);

    mQueueSet = xQueueCreateSet(queueSetSize);
    while(count--) {
        handle = va_arg(vl, void*);
        xQueueAddToSet( handle, mQueueSet);
    }

    va_end(vl);
}
#endif

bool  scheduler_task::addSharedObject(const char *name, void *obj_ptr)
{
    bool duplicate = false;

    vPortEnterCritical();
    do {
        if (!mpNamePtrPair) {
            mpNamePtrPair = new VECTOR<PtrNamePairType>;
        }

        /* Disallow adding duplicate items by name */
        for(unsigned int i=0; i < mpNamePtrPair->size(); i++) {
            if (0 == strcmp(mpNamePtrPair->at(i).name, name)) {
                duplicate = true;
                break;
            }
        }

        /* Push back to vector */
        PtrNamePairType item;
        item.ptr  = obj_ptr;
        item.name = name;
        mpNamePtrPair->push_back(item);
    } while(0);
    vPortExitCritical();

    return !duplicate;
}
void* scheduler_task::getSharedObject(const char *name)
{
    // We can only access the vector if someone has already called addSharedObject()
    if(!mpNamePtrPair) {
        return NULL;
    }

    for(unsigned int i=0; i < mpNamePtrPair->size(); i++) {
        const PtrNamePairType& item = mpNamePtrPair->at(i);
        if (0 == strcmp(item.name,name)) {
            return item.ptr;
        }
    }

    return NULL;
}
