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

#include <stdio.h>      // printf
#include <string.h>     // memset() etc.

#include "FreeRTOS.h"
#include "semphr.h"

#include "uart0.hpp"            // Interrupt driven UART0 driver
#include "lpc_sys.h"            // Set input/output char functions
#include "utilities.h"          // PRINT_EXECUTION_SPEED()
#include "handlers.hpp"         // Command-line handlers

#include "file_logger.hpp"      // Logging
#include "io.hpp"               // For Board IO
#include "tasks.hpp"

#include "c_tlm_var.h"
#include "c_tlm_comp.h"
#include "c_tlm_stream.h"
#include "c_tlm_binary.h"


///< Max characters for command-line input
#define MAX_COMMANDLINE_INPUT   128



/**
 * Gets a line of input with backspace support and stores into str s
 */
UartDev* getCommand(str &cmd);

terminalTask::terminalTask(uint8_t priority) :
        scheduler_task("terminal", 1024*5, priority),
        mCommandCount(0), mDiskTlmSize(0), mpBinaryDiskTlm(NULL),
        mCmdProc(18),
        mTerminalCmd(MAX_COMMANDLINE_INPUT)
{
    /* Nothing to do */
}

bool terminalTask::regTlm(void)
{
    #if ENABLE_TELEMETRY
    return (TLM_REG_VAR(tlm_component_get_by_name("debug"), mCommandCount, tlm_uint) &&
            TLM_REG_VAR(tlm_component_get_by_name("debug"), mDiskTlmSize, tlm_uint));
    #else
    return true;
    #endif
}

bool terminalTask::taskEntry()
{
    /* remoteTask() creates shared object in its init(), so we can get it now */
    xSemaphoreHandle learn_sem = getSharedObject("learn_sem");
    CommandProcessor &cp = mCmdProc;

    // System information handlers
    cp.addHandler(taskListHandler, "info",    "Task/CPU Info.  Use 'info 200' to get CPU during 200ms");
    cp.addHandler(memInfoHandler,  "meminfo", "See memory info");
    cp.addHandler(healthHandler,   "health",  "Output system health");
    cp.addHandler(timeHandler,     "time",    "'time' to view time.  'time set MM DD YYYY HH MM SS Wday' to set time");

    // File I/O handlers:
    cp.addHandler(catHandler,    "cat",   "Read a file.  Ex: 'cat 0:file.txt' or "
                                          "'cat 0:file.txt -noprint' to test if file can be read");
    cp.addHandler(cpHandler,     "cp",    "Copy files from/to Flash/SD Card.  Ex: 'cp 0:file.txt 1:file.txt'");
    cp.addHandler(dcpHandler,    "dcp",   "Copy all files of a directory to another directory.  Ex: 'dcp 0:src 1:dst'");
    cp.addHandler(lsHandler,     "ls",    "Use 'ls 0:' for Flash, or 'ls 1:' for SD Card");
    cp.addHandler(mkdirHandler,  "mkdir", "Create a directory. Ex: 'mkdir test'");
    cp.addHandler(mvHandler,     "mv",    "Rename a file. Ex: 'rm 0:file.txt 0:new.txt'");
    cp.addHandler(newFileHandler,"nf",    "Write a new file. Ex: 'nf <file.txt>");
    cp.addHandler(rmHandler,     "rm",    "Remove a file. Ex: 'rm 0:file.txt'");

    // Misc. handlers
    cp.addHandler(storageHandler,  "storage",  "Parameters: 'format sd', 'format flash', 'mount sd', 'mount flash'");
    cp.addHandler(rebootHandler,   "reboot",   "Reboots the system");
    cp.addHandler(logHandler,      "log",      "'log <hello>' to log info.  'log flush' to flush logs");
    cp.addHandler(learnIrHandler,  "learn",    "Begin to learn IR codes for numbers 0-9", learn_sem, sizeof(learn_sem));
    cp.addHandler(wirelessHandler, "wireless", "'wireless rx <ms>' : Poll for wireless packet\n"
                                               "'wireless addr #'  : Set wireless address\n"
                                               "'wireless routes'  : Get list of wireless nodes\n"
                                               "'wireless stats'   : Get statistics (rx/tx counters)\n"
                                               "'wireless ack ## <msg> : Send ack  message to a node\n"
                                               "'wireless nack # <msg> : Send nack message to a node");

    #if (ENABLE_TELEMETRY)
    cp.addHandler(telemetryHandler, "telemetry", "Outputs registered telemetry: "
                                                 "'telemetry save' : Saves disk tel\n"
                                                 "'telemetry <comp. name> <name> <value>' to set a telemetry variable\n"
                                                 "'telemetry get <comp. name> <name>' to get variable value\n");
    #endif

    // Initialize Interrupt driven version of getchar & putchar
    Uart0& uart0 = Uart0::getInstance();
    bool success = uart0.init(38400, 32, 512);
    sys_set_inchar_func(uart0.getcharIntrDriven);
    sys_set_outchar_func(uart0.putcharIntrDriven);

    #if ENABLE_TELEMETRY
    /* Telemetry should be registered at this point, so initialize the binary
     * telemetry space that we periodically check to save data to disk
     */
    tlm_component *disk = tlm_component_get_by_name(DISK_TLM_NAME);
    mDiskTlmSize = tlm_binary_get_size_one(disk);
    mpBinaryDiskTlm = new char[mDiskTlmSize];
    if (success) {
        success = (NULL != mpBinaryDiskTlm);
    }

    /* Now update our copy of disk telemetry */
    tlm_binary_get_one(disk, mpBinaryDiskTlm);
    #endif

    STR_ON_STACK(help, 8);
    help = "help";
    mCmdProc.handleCommand(help, uart0);

    return success;
}

bool terminalTask::run(void* p)
{
    printf("LPC: ");

    // If no command (idle), then use this time to save disk telemetry
    UartDev* out = getCommand(mTerminalCmd);
    if (!out) {
        puts("");
        saveDiskTlm();          ///< This may print a message
        printf(mTerminalCmd()); ///< Re-display what the user entered
    }
    else if (mTerminalCmd.getLen() > 0)
    {
        PRINT_EXECUTION_SPEED()
        {
            ++mCommandCount;
            mCmdProc.handleCommand(mTerminalCmd, *out);

            /* Send special chars to indicate end of command output
             * Usually, serial terminals will ignore these chars
             */
            const char endOfTx[] = { 3, 3, 4, 4 };
            for (unsigned i = 0; i < sizeof(endOfTx); i++) {
                out->putChar(endOfTx[i]);
            }
        }
        mTerminalCmd.clear();
    }

    return true;
}

bool terminalTask::saveDiskTlm(void)
{
    #if ENABLE_TELEMETRY
    tlm_component *disk = tlm_component_get_by_name(DISK_TLM_NAME);

    /* Size of telemetry shouldn't change */
    if (mDiskTlmSize != tlm_binary_get_size_one(disk)) {
        return false;
    }

    if (!tlm_binary_compare_one(disk, mpBinaryDiskTlm)) {
        puts("Disk variables changed...");

        FILE *file = fopen(DISK_TLM_NAME, "w");
        if (file) {
            // Only update variables if we could open the file
            tlm_binary_get_one(disk, mpBinaryDiskTlm);

            tlm_stream_one_file(disk, file);
            fclose(file);

            puts("Changes saved to disk...");
            LOG_INFO_SIMPLE("Disk variables saved to disk");
        } else {
            return false;
        }
    }
    #endif

    return true;
}

void terminalTask::handleEchoAndBackspace(str& cmd, char newChar)
{
    /* Echo & backspace logic over stdio */
    // Backspace 1 char @ terminal and erase last char of string
    if ('\b' == newChar) {
        if (cmd.getLen() > 0) {
            putchar('\b');
            putchar(' ');
            cmd.eraseLast(1);
        }
        else {
            // Send "Alert" sound to terminal because we can't backspace
            const char bellSound = '\a';
            newChar =  bellSound;
        }
    }
    else if ('\n' != newChar && '\r' != newChar) {
        cmd += newChar;
    }
    putchar(newChar);
}

UartDev* terminalTask::getCommand(str &cmd)
{
    Uart0& u0 = Uart0::getInstance();
    UartDev* ret = &u0;
    char c = 0;

    do
    {
        /* If no command for 60 secs, return NULL back to give the task a chance
         * to do any misc. activities.
         */
        if (u0.getChar(&c, 100)) {
            handleEchoAndBackspace(cmd, c);
        }
        else if (!u0.recentlyActive(60*1000)) {
            u0.resetActivity();
            ret = NULL;
            break;
        }

        /**
         * Guard against command length too large
         */
        if (cmd.getLen() >= cmd.getCapacity() - 1) {
            break;
        }
    } while (c != '\n');

    return ret;
}
