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

#include <stdio.h>              // printf()
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"               // vTaskList()

#include "command_handler.hpp"  // CMD_HANDLER_FUNC()
#include "rtc.h"                // Set and Get System Time
#include "lpc_sys.h"

#include "utilities.h"          // printMemoryInfo()
#include "storage.hpp"          // Get Storage Device instances
#include "file_logger.hpp"      // Logger class

#include "wireless.h"
#include "uart0.hpp"
#include "io.hpp"
#include "c_tlm_stream.h"
#include "c_tlm_var.h"



CMD_HANDLER_FUNC(taskListHandler)
{
    const int delayInMs = (int)cmdParams;  // cast parameter str to integer

    if(delayInMs > 0) {
        vTaskResetRunTimeStats();
        vTaskDelayMs(delayInMs);
    }

    // Enum to char : eRunning, eReady, eBlocked, eSuspended, eDeleted
    const char taskStatusTbl[] = { 'N', 'R', 'B', 'S', 'D' };

    // Limit the tasks to avoid heap allocation.
    const unsigned portBASE_TYPE maxTasks = 10;
    xTaskStatusType status[maxTasks];
    unsigned long totalRunTime = 0;
    const unsigned portBASE_TYPE uxArraySize =
            uxTaskGetSystemState(&status[0], maxTasks, &totalRunTime);

    // For percentage calculations.
    totalRunTime /= 100UL;

    output.printf("%10s St Pr  Stack  CPU%%  Ticks(%u uS/tick)\n", "Name", TIMER0_US_PER_TICK);
    for(unsigned portBASE_TYPE i = 0; i < uxArraySize; i++) {
        xTaskStatusType *e = &status[i];
        const unsigned int cpuPercent = (0 == totalRunTime) ? 0 : e->ulRunTimeCounter / totalRunTime;

        output.printf("%10s  %c %2u  %5u  %4u  %lu\n",
                      e->pcTaskName, taskStatusTbl[e->eCurrentState], e->uxBasePriority,
                      e->usStackHighWaterMark, cpuPercent, e->ulRunTimeCounter);
    }

    if (uxTaskGetNumberOfTasks() > maxTasks) {
        output.printf("** WARNING: Only reported first %u tasks\n", maxTasks);
    }
    return true;
}

CMD_HANDLER_FUNC(memInfoHandler)
{
    char buffer[128] = { 0 };
    sys_get_mem_info_str(buffer);
    output.putline(buffer);
    return true;
}

CMD_HANDLER_FUNC(healthHandler)
{
    Uart0 &u0 = Uart0::getInstance();

    unsigned int total = 0;
    unsigned int available = 0;
    Storage::getFlashDrive().getDriveInfo(&total, &available);

    float floatTemp = TS.getFarenheit();
    int floatSig1 = (int) floatTemp;
    int floatDec1 = ((floatTemp - floatSig1) * 10);
    rtc_t bt = sys_get_boot_time();

    char dateTime[24] = { 0 };
    rtc_get_date_time_str(dateTime);

    output.printf("Flash: %u/%u\n"
                    "Temp : %u.%u\n"
                    "Light: %u\n"
                    "Time     : %s\n"
                    "Boot Time: %u/%u/%u,%02u:%02u:%02u\n"
                    "Uart0 Watermarks: %u/%u (rx/tx)\n",
                    available, total,
                    floatSig1, floatDec1,
                    LS.getRawValue(),
                    dateTime,
                    bt.month, bt.day, bt.year, bt.hour, bt.min, bt.sec,
                    u0.getRxQueueWatermark(), u0.getTxQueueWatermark()
    );
    return true;
}

CMD_HANDLER_FUNC(timeHandler)
{
    rtc_t time;

    /**
     * If cmdParam contains "set" with six spaces, we can parse the time
     * Example: set 11 30 2014 8 25 0 1
     */
    if(cmdParams.beginsWith("set"))
    {
        unsigned m, d, y, hr, mn, sc, w;
        if( 7 != cmdParams.scanf("%*s %u %u %u %u %u %u %u", &m, &d, &y, &hr, &mn, &sc, &w))
        {
            return false;
        }

        time.month = m; time.day = d; time.year = y;
        time.hour = hr, time.min = mn; time.sec = sc;
        time.dow = w;
        rtc_settime(&time);
    }

    time = rtc_gettime();
    output.printf("%02u/%02u/%u  --  %02u:%02u:%02u (%u)\n",
                    time.month, time.day, time.year,
                    time.hour, time.min, time.sec, time.dow);
    return true;
}

CMD_HANDLER_FUNC(logHandler)
{
    if (cmdParams == "flush") {
        FileLogger::getInstance().flush();
        output.putline("Log(s) have been flushed");
    }
    else {
        LOG_INFO(cmdParams());
        output.printf("Logged: |%s|\n", cmdParams());
    }
    return true;
}

CMD_HANDLER_FUNC(cpHandler)
{
    char *srcFile = NULL;
    char *dstFile = NULL;
    if(2 != cmdParams.tokenize(" ", 2, &srcFile, &dstFile)) {
        return false;
    }

    unsigned int readTimeMs = 0;
    unsigned int writeTimeMs = 0;
    unsigned int bytesTransferred = 0;
    FRESULT copyStatus = Storage::copy(srcFile, dstFile,
                                       &readTimeMs, &writeTimeMs, &bytesTransferred);

    if(FR_OK != copyStatus) {
        output.printf("Error %u copying |%s| -> |%s|\n", copyStatus, srcFile, dstFile);
    }
    else {
        output.printf("Finished!  Read: %u Kb/sec, Write: %u Kb/sec\n",
                      bytesTransferred/(0 == readTimeMs  ? 1 : readTimeMs),
                      bytesTransferred/(0 == writeTimeMs ? 1 : writeTimeMs));
    }
    return true;
}

CMD_HANDLER_FUNC(catHandler)
{
    // If -print was present, we will print to console
    const bool printToScreen = !cmdParams.erase("-noprint");
    cmdParams.trimStart(" ");
    cmdParams.trimEnd(" ");

    FIL file;
    if(FR_OK != f_open(&file, cmdParams(), FA_OPEN_EXISTING | FA_READ))
    {
        output.printf("Failed to open: %s\n", cmdParams());
    }
    else
    {
        // Extra char for null terminator
        char buffer[1024 + 1] = { 0 };
        unsigned int bytesRead = 0;
        unsigned int totalBytesRead = 0;

        const unsigned int startTime = sys_get_uptime_ms();
        while(FR_OK == f_read(&file, buffer, sizeof(buffer)-1, &bytesRead) && bytesRead > 0)
        {
            totalBytesRead += bytesRead;
            buffer[bytesRead] = '\0';

            if(printToScreen) {
                output.put(buffer);
            }
        }
        f_close(&file);

        if(!printToScreen) {
            const unsigned int timeTaken = sys_get_uptime_ms() - startTime;
            output.printf("\nRead %d bytes @ %d Kb/sec", totalBytesRead,
                          totalBytesRead/(0 == timeTaken  ? 1 : timeTaken));
        }
        output.putline("");
    }
    return true;
}

CMD_HANDLER_FUNC(lsHandler)
{
    DIR Dir;
    FILINFO Finfo;
    FATFS *fs;
    FRESULT returnCode = FR_OK;

    unsigned int fileBytesTotal = 0, numFiles = 0, numDirs = 0;
    #if _USE_LFN
        char Lfname[_MAX_LFN];
    #endif

    char *dirPath = NULL;
    char *lsOffsetStr = NULL;
    int lsOffset = 0;
    if (2 == cmdParams.tokenize(" ", 2, &dirPath, &lsOffsetStr)) {
        lsOffset = atoi(lsOffsetStr);
    }

    if (NULL == dirPath) {
        dirPath = (char*)"0:";
    }

    if (FR_OK != (returnCode = f_opendir(&Dir, dirPath))) {
        output.printf("Invalid directory: |%s| (Error %i)\n", dirPath, returnCode);
        return true;
    }

    // Offset the listing
    while(lsOffset-- > 0) {
        #if _USE_LFN
            Finfo.lfname = Lfname;
            Finfo.lfsize = sizeof(Lfname);
        #endif
        if (FR_OK != f_readdir(&Dir, &Finfo)) {
            break;
        }
    }

    output.printf("Directory listing of: %s\n\n", dirPath);
    for (;;)
    {
        #if _USE_LFN
            Finfo.lfname = Lfname;
            Finfo.lfsize = sizeof(Lfname);
        #endif

        char returnCode = f_readdir(&Dir, &Finfo);
        if ( (FR_OK != returnCode) || !Finfo.fname[0]) {
            break;
        }

        if (Finfo.fattrib & AM_DIR){
            numDirs++;
        }
        else{
            numFiles++;
            fileBytesTotal += Finfo.fsize;
        }
        output.printf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %10lu %13s",
                (Finfo.fattrib & AM_DIR) ? 'D' : '-',
                (Finfo.fattrib & AM_RDO) ? 'R' : '-',
                (Finfo.fattrib & AM_HID) ? 'H' : '-',
                (Finfo.fattrib & AM_SYS) ? 'S' : '-',
                (Finfo.fattrib & AM_ARC) ? 'A' : '-',
                (Finfo.fdate >> 9) + 1980, (Finfo.fdate >> 5) & 15, Finfo.fdate & 31,
                (Finfo.ftime >> 11), (Finfo.ftime >> 5) & 63,
                Finfo.fsize, &(Finfo.fname[0]));

        // LFN names tend to increase memory requirements for output str, enable with caution
        #if (_USE_LFN)
        output.put(" - ");
        output.put(Lfname);
        #endif
        output.putline("");
    }
    output.printf("\n%4u File(s), %10d bytes total\n%4d Dir(s)", numFiles, fileBytesTotal, numDirs);

    if (f_getfree(dirPath, (DWORD*) &fileBytesTotal, &fs) == FR_OK)
    {
        output.printf(", %10dK bytes free\n", fileBytesTotal * fs->csize / 2);
    }
    return true;
}

CMD_HANDLER_FUNC(mkdirHandler)
{
    output.printf("Create directory '%s' : %s\n",
                   cmdParams(), (FR_OK == f_mkdir(cmdParams())) ? "OK" : "ERROR");
    return true;
}

CMD_HANDLER_FUNC(rmHandler)
{
    output.printf("Delete '%s' : %s\n",
                  cmdParams(), (FR_OK == f_unlink(cmdParams())) ? "OK" : "ERROR");
    return true;
}

CMD_HANDLER_FUNC(mvHandler)
{
    char *srcFile = NULL;
    char *dstFile = NULL;
    if(2 != cmdParams.tokenize(" ", 2, &srcFile, &dstFile)) {
        return false;
    }
    else {
        output.printf("Move '%s' -> '%s' : %s\n",
                      srcFile, dstFile,
                      (FR_OK == f_rename(srcFile, dstFile))  ? "OK" : "ERROR");
    }
    return true;
}

CMD_HANDLER_FUNC(newFileHandler)
{
    const char end_file = '~';
    int timeout_ms = OS_MS(10 * 1000);

    FIL file;
    if (FR_OK != f_open(&file, cmdParams(), FA_WRITE | FA_CREATE_ALWAYS)) {
        output.printf("Unable to open '%s' to write the file\n", cmdParams());
        return true;
    }

    char c = 0;
    UINT bw = 0;
    output.printf("End the file by using %c character.  %i is the timeout\n", end_file, timeout_ms);
    output.printf("Sorry, no backspace support :(\n");

    while (output.getChar(&c, timeout_ms) && c != end_file) {
        if (FR_OK != f_write(&file, &c, 1, &bw) && 1 != bw) {
            output.printf("Error occurred while writing the file\n");
        }
        else {
            output.putChar(c);
        }
    }

    f_close(&file);
    return true;
}

CMD_HANDLER_FUNC(dcpHandler)
{
    DIR Dir;
    FILINFO Finfo;
    FRESULT returnCode = FR_OK;
    #if _USE_LFN
        char Lfname[_MAX_LFN];
    #endif

    char *srcDir = NULL;
    char *dstDir = NULL;
    if(2 != cmdParams.tokenize(" ", 2, &srcDir, &dstDir)) {
        output.putline("ERROR: Give me source and destination directory separated by a space");
        return true;
    }

    /* Check destination directory */
    if (FR_OK != (returnCode = f_opendir(&Dir, dstDir))) {
        output.printf("Invalid destination directory: |%s|\n", dstDir);
        return true;
    }
    /* Check source directory */
    if (FR_OK != (returnCode = f_opendir(&Dir, srcDir))) {
        output.printf("Invalid source directory: |%s|\n", srcDir);
        return true;
    }

    STR_ON_STACK(src, 256);
    STR_ON_STACK(dst, 256);
    for (;;)
    {
        #if _USE_LFN
            Finfo.lfname = Lfname;
            Finfo.lfsize = sizeof(Lfname);
        #endif

        /* If no more files */
        if ( (FR_OK != f_readdir(&Dir, &Finfo)) || !Finfo.fname[0]) {
            break;
        }

        /* If not a directory */
        if (!(Finfo.fattrib & AM_DIR))
        {
            src.printf("%s/%s", srcDir, Finfo.fname);
            dst.printf("%s/%s", dstDir, Finfo.fname);

            output.printf("Copy %s -> %s : %d Bytes : %s\n",
                           src(), dst(), Finfo.fsize,
                           (FR_OK == Storage::copy(src(), dst())) ? "OK" : "ERROR");
        }
    }
    return true;
}

CMD_HANDLER_FUNC(storageHandler)
{
    if(cmdParams == "format sd") {
        output.putline((FR_OK == Storage::getSDDrive().format()) ? "Format OK" : "Format ERROR");
    }
    else if(cmdParams == "format flash") {
        output.putline((FR_OK == Storage::getFlashDrive().format()) ? "Format OK" : "Format ERROR");
    }
    else if(cmdParams == "mount sd") {
        output.putline(FR_OK == Storage::getSDDrive().mount() ? "SD Card mounted" : "Error mounting SD Card");
    }
    else if(cmdParams == "mount flash") {
        output.putline(FR_OK == Storage::getFlashDrive().mount() ? "Flash mounted" : "Error mounting Flash Memory");
    }
    else {
        return false;
    }
    return true;
}

CMD_HANDLER_FUNC(rebootHandler)
{
    output.putline("Rebooting System");

    // Flush out everything before we reboot
    FileLogger::getInstance().flush();

    vTaskDelayMs(2000);
    sys_reboot();
    return true;
}

#if (ENABLE_TELEMETRY)
CMD_HANDLER_FUNC(telemetryHandler)
{
    if(cmdParams.getLen() == 0) {
        tlm_stream_all_file(stdout);
    }
    else if(cmdParams == "save") {
        FILE *fd = fopen(DISK_TLM_NAME, "w");
        tlm_stream_one_file(tlm_component_get_by_name(DISK_TLM_NAME), fd);
        fclose(fd);
        output.putline("Telemetry was saved to disk");
    }
    else if(cmdParams.beginsWithIgnoreCase("get")) {
        char *compName = NULL;
        char *varName = NULL;
        if (3 == cmdParams.tokenize(" ", 3, NULL, &compName, &varName)) {
            char buffer[256] = { 0 };
            if (tlm_variable_get_value(compName, varName, &buffer[0], sizeof(buffer))) {
                output.putline(buffer);
            }
            else {
                output.putline("Error locating or printing variable value");
            }
        }
        else {
            output.putline("Required parameters: 'get <comp name> <var name>");
        }
    }
    else {
        char *compName = NULL;
        char *varName = NULL;
        char *varVal = NULL;
        if (3 != cmdParams.tokenize(" ", 3, &compName, &varName, &varVal)) {
            output.putline("ERROR: See 'help telemetry'");
        }
        else {
            if (tlm_variable_set_value(compName, varName, varVal)) {
                output.printf("%s:%s set to %s\n", compName, varName, varVal);
            } else {
                output.printf("Failed to set %s:%s to %s\n", compName, varName, varVal);
            }
        }
    }
    return true;
}
#endif

CMD_HANDLER_FUNC(learnIrHandler)
{
    // Learning semaphore is passed as parameter when command was registered
    // The other task will wake up to perform IR learning
    xSemaphoreHandle learn_sem = pDataParam;
    xSemaphoreGive(learn_sem);
    output.putline("Learning mode enabled");
    return true;
}

#if MESH_USE_STATISTICS
static void wirelessHandlerPrintStats(UartDev& output, mesh_stats_t *s, uint8_t node)
{
    output.printf("N%u: Rx/Tx, Rte/Ovt, Retried/Mesh Retried/Repeated: \n", node);
    output.printf("       %3u/%-3u %u/%u, %u/%u/%u\n",
                    s->pkts_intercepted, s->pkts_sent,
                    s->rte_entries, s->rte_overwritten,
                    s->pkts_retried, s->pkts_retried_others, s->pkts_repeated);
}
#endif

CMD_HANDLER_FUNC(wirelessHandler)
{
    char *addr_str = NULL;
    char *data_str = NULL;
    const int max_hops_to_use = 2;
    int timeout_ms = 1000;
    mesh_packet_t pkt;
#if MESH_USE_STATISTICS
    mesh_stats_t stats = { 0 };
#endif

    if (cmdParams.beginsWithIgnoreCase("rx")) {
        // Even if we can't scanf, we will default to timeout's value
        cmdParams.scanf("%*s %i", &timeout_ms);

        if (wireless_get_rx_pkt(&pkt, timeout_ms)) {
            output.printf("Received data from %i\n", pkt.nwk.src);
        }
        else {
            output.putline("No data received");
        }
    }
    else if (cmdParams.beginsWithIgnoreCase("addr")) {
        int addr = 0;
        cmdParams.scanf("%*s %i", &addr);
        output.printf("Set address to %i: %s\n", addr, mesh_set_node_address(addr) ? "OK" : "FAILED");
    }
    else if (cmdParams.compareToIgnoreCase("routes")) {
        output.printf("Routing table size is %i\n", mesh_get_num_routing_entries());

        const mesh_rte_table_t *e = NULL;
        uint8_t i = 0;
        while ((e = mesh_get_routing_entry(i++))) {
            output.printf("DST: %3i, Next Hop: %3i, Hops: %i\n", e->dst, e->next_hop, e->num_hops);
        }
    }
#if MESH_USE_STATISTICS
    else if (cmdParams.compareToIgnoreCase("stats")) {
        stats = mesh_get_stats();
        wirelessHandlerPrintStats(output, &stats, mesh_get_node_address());
    }
#endif
    else if ((cmdParams.beginsWithIgnoreCase("ack") || cmdParams.beginsWithIgnoreCase("nack")) &&
             cmdParams.tokenize(" ", 3, NULL, &addr_str, &data_str) >= 2)
    {
        const bool ack = cmdParams.beginsWithIgnoreCase("ack");

        /* Data is optional */
        const uint8_t dst_addr = atoi(addr_str);
        const uint8_t len = data_str ? strlen(data_str) : 0;

        // Flush any packets
        while (wireless_get_rx_pkt(&pkt, 0)) {
            output.putline("Discarded a stale wireless packet");
            ;
        }

        if (! wireless_send(dst_addr, ack ? mesh_pkt_ack : mesh_pkt_nack, data_str, len, max_hops_to_use)) {
            output.putline("Error sending packet, check parameters!");
        }
        /* If ack was requested, then we wait for the ack */
        else if (ack)
        {
            if(wireless_get_ack_pkt(&pkt, timeout_ms) && dst_addr == pkt.nwk.src)
            {
                #if MESH_USE_STATISTICS
                if (sizeof(stats) == pkt.info.data_len) {
                    mesh_stats_t *p = (mesh_stats_t*) &(pkt.data[0]);
                    wirelessHandlerPrintStats(output, p, pkt.nwk.src);
                }
                #endif

                /* Response to ping packet, so print the node name: */
                if (0 == len) {
                    output.printf("Remote node name: '");
                    for (int i=0; i<pkt.info.data_len; i++) {
                        output.putChar(pkt.data[i]);
                    }
                    output.printf("'\n");
                }
                else {
                    output.putline("Received the acknowledgment!");
                }
            }
            else {
                output.printf("Packet sent to %s but no ACK received", addr_str);
            }
        }
    }
    else {
        return false;
    }
    return true;
}
