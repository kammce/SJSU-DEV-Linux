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

#include <stdio.h>          // printf
#include <string.h>
#include <stdlib.h>

#include "lpc_sys.h"
#include "sys_config.h"     // CPU Clock Configuration
#include "utilities.h"

#include "fat/disk/sd.h"        // Initialize SD Card Pins for CS, WP, and CD
#include "fat/disk/spi_flash.h" // Initialize Flash CS pin

#include "rtc.h"             // RTC init
#include "i2c2.hpp"          // I2C2 init
#include "adc0.h"            // ADC0 init
#include "spi0.h"            // SPI-0 init
#include "spi1.h"            // SPI-1 init

#include "storage.hpp"       // Mount Flash & SD Storage
#include "bio.h"             // Init io signals
#include "io.hpp"            // Board IO peripherals

#include "wireless.h"
#include "fault_registers.h"
#include "c_tlm_comp.h"



static void hl_periodic_service(void);
static bool hl_mount_storage(FileSystemObject& drive, const char* pDescStr);
static void hl_move_log_file(void);
static bool hl_discover_external_i2c_devs(void);
static bool hl_init_board_io(void);
static void wireless_set_addr_from_file(void);
static void hl_print_line() { puts("------------------------------------------------------"); }



static volatile uint64_t m_system_uptime_ms = 0;
static const unsigned int m_time_per_rit_isr_ms = 1;
#if (1 != configUSE_TICK_HOOK)
#error "configUSE_TICK_HOOK is required to be 1 to provide system timer service"
#endif
extern "C"
{
    // This function is called at every FreeRTOS OS Tick
    void vApplicationTickHook( void )
    {
        hl_periodic_service();
    }
    uint64_t sys_get_uptime_ms(void)
    {
        return m_system_uptime_ms;
    }
}

/* If FreeRTOS is not running, then we rely on rit service to call this function,
 * otherwise we rely on FreeRTOS tick hook to provide system timer
 */
static void hl_periodic_service(void)
{
    const uint32_t timer_ms = sys_get_uptime_ms();

    /* If FreeRTOS is running, user should use a dedicated task to call mesh service,
     * so we will not call it if freertos is running
     */
    if (taskSCHEDULER_RUNNING == xTaskGetSchedulerState()) {
        m_system_uptime_ms += MS_PER_TICK();

        /* We don't need RIT if FreeRTOS is running */
        if (sys_rit_running()) {
            sys_rit_disable();

            /* Round up uptime_ms because if ms per tick is 10, then we don't want to
             * increment this timer by 10 from an odd number because % 10 won't work.
             */
            m_system_uptime_ms = (m_system_uptime_ms / 10) * 10;
        }
    }
    else {
        m_system_uptime_ms += m_time_per_rit_isr_ms;
        wireless_service();

        /**
         * Small hack to support interrupts if FreeRTOS is not running :
         * FreeRTOS API resets our base priority register, then all
         * interrupts higher priority than IP_SYSCALL will not get locked out.
         *   @see more notes at isr_priorities.h.  @see IP_SYSCALL
         */
        __set_BASEPRI(0);
    }
	
	/**
     * Call SD timer function at 100Hz.
     * Feed the watchdog too while we're at it.
     */
    if (0 == (timer_ms % 10)) {
        sd_timerproc();
        sys_watchdog_feed();
    }
}



/**
 * Initializes the High Level System such as IO Pins and Drivers
 */
void high_level_init(void)
{
    // Initialize all board pins (early) that are connected internally.
    board_io_pins_initialize();
    
    #if ENABLE_TELEMETRY
        /* Add default telemetry components */
        tlm_component_add("disk");
        tlm_component_add("debug");
    #endif

    /**
     * Set-up Timer0 so that delay_ms(us) functions will work.
     * This function is used by FreeRTOS run time statistics.
     * If FreeRTOS is used, timer will be set-up anyway.
     * If FreeRTOS is not used, call it here such that delay_ms(us) functions will work.
     */
    vConfigureTimerForRunTimeStats();

    /**
     * Intentional delay here because this gives the user some time to
     * close COM Port at Hyperload and Open it at Hercules
     */
    delay_ms(STARTUP_DELAY_MS);
    hl_print_line();

    /* Print boot info */
    const unsigned int cpuClock = sys_get_cpu_clock();
    const unsigned int sig = cpuClock / (1000 * 1000);
    const unsigned int fraction = (cpuClock - (sig*1000*1000)) / 1000;;
    printf("\nSystem Boot @ %u.%u Mhz\n", sig, fraction);

    if(SysBootType_watchdog_recover == sys_get_boot_type()) {
        hl_print_line();
        printf("System rebooted after crash.  Relevant info:\n"
               "PC: 0x%08X.  LR: 0x%08X.  PSR: 0x%08X\n",
                (unsigned int)FAULT_PC, (unsigned int)FAULT_LR, (unsigned int)FAULT_PSR);
        hl_print_line();
    }

    /**
     * Initialize the Peripherals used in the system
     * I2C2 : Used by LED Display, Acceleration Sensor, Temperature Sensor
     * ADC0 : Used by Light Sensor
     * SPI0 : Used by Nordic
     * SPI1 : Used by SD Card & External SPI Flash Memory
     */
    adc0_init();
    spi1_init();
    spi0_init(SPI0_CLOCK_SPEED_MHZ);
    if (!I2C2::getInstance().init(I2C2_CLOCK_SPEED_KHZ)) {
        puts("I2C #2: ERROR: Possible short on SDA or SCL wire!");
    }

    /* Initialize nordic wireless mesh network before setting up sys_setup_rit()
     * callback since it may access NULL function pointers.
     */
    if (!wireless_init()) {
        puts("ERROR: Failed to initialize wireless");
    }

    /* Set-up our RIT callback to perform some background book-keeping
     * If FreeRTOS is running, this service is disabled and FreeRTOS
     * tick hook will call hl_periodic_service()
     * @warning RIT interrupt should be setup before SD card is mounted
     *           since it relies on our timer.
     */
    sys_rit_setup(hl_periodic_service, m_time_per_rit_isr_ms);

    /**
     * If Flash is not mounted, it is probably a new board and the flash is not
     * formatted so format it, alert the user, and try to re-mount it
     */
    bool flashMounted = hl_mount_storage(Storage::getFlashDrive(), " Flash ");
    if(!flashMounted)
    {
        printf("FLASH not formatted, formatting now ... ");
        printf("%s\n", FR_OK == Storage::getFlashDrive().format() ? "Done" : "Error");
        flashMounted = hl_mount_storage(Storage::getFlashDrive(), " Flash ");
    }

    /**
     * If SD Card is present, try to copy log file from Flash to SD Card
     */
    const bool sdMounted = hl_mount_storage(Storage::getSDDrive(),    "SD Card");
    if(flashMounted && sdMounted)
    {
        hl_move_log_file();
    }

	/* After SD card is initted, set desired speed for spi1 */
    spi1_set_max_clock(SPI1_CLOCK_SPEED_MHZ);
    hl_print_line();

    #if LOG_BOOT_INFO_TO_FILE
    log_boot_info(__DATE__);
    #endif

    /**
     * Print discovered I2C devices.
     * But not really useful if no external I2C device attached.
     * If we do find an external device though, we will print a line after the output.
     */
    if(hl_discover_external_i2c_devs()) {
        hl_print_line();
    }

    /* Initialize all sensors of this board. */
    if(!hl_init_board_io()) {
        hl_print_line();
        LD.setLeftDigit('-');
        LD.setRightDigit('-');
        LE.setAll(0xFF);
    }
    else {
        LD.setNumber(TS.getFarenheit());
    }

    /* Print memory information before we call main() */
    do {
        char buff[512] = { 0 };
        sys_get_mem_info_str(buff);
        puts(buff);
        hl_print_line();
    } while(0);

    /* After Flash memory is mounted, try to set node address from a file */
    wireless_set_addr_from_file();
    srand(LS.getRawValue());

    // Display CPU speed in Mhz on LED display
    // LD.setNumber(sys_get_cpu_clock()/(1000*1000));

    puts("Calling your main()");
    hl_print_line();
}

static bool hl_mount_storage(FileSystemObject& drive, const char* pDescStr)
{
    unsigned int totalKb = 0;
    unsigned int availKb = 0;
    const char st = drive.mount();
    bool mounted = (0 == st);

    if(mounted && FR_OK == drive.getDriveInfo(&totalKb, &availKb))
    {
        if(totalKb < 1000)
            printf("%s: OK -- Capacity %-5dKB, Available: %-5uKB\n", pDescStr, totalKb, availKb);
        else
            printf("%s: OK -- Capacity %-5dMB, Available: %-5uMB\n", pDescStr, totalKb/1024, availKb/1024);
    }
    else {
        printf("%s: Error or not present.  Error #%i, Mounted: %s\n", pDescStr, st, mounted ? "Yes" : "No");
        mounted = false;
    }

    return mounted;
}

static void hl_move_log_file(void)
{
    // Open destination file with same filename but store into drive '1' for SD Card
    char dstFileName[sizeof(LOGGER_FILE_NAME) + 3] = { 0 };
    strcpy(dstFileName, "1:");
    strcat(dstFileName, LOGGER_FILE_NAME);

    printf("Attempt to copy %s to SD Card ... ", (char*)LOGGER_FILE_NAME);

    // Copy log file from Flash to SD Card:
    FRESULT copyStatus = Storage::copy(LOGGER_FILE_NAME, dstFileName);

    // Delete logger file from FLASH (if you want to)
    #if DEL_UPON_LOGFILE_MOVE_TO_SD
    if(FR_OK == copyStatus) {
        f_unlink(LOGGER_FILE_NAME);
    }
    #endif

    printf("%s\n", (FR_OK == copyStatus) ? "Done!" : (FR_NO_FILE == copyStatus) ? "Not found" : "ERROR!");
}

static bool hl_discover_external_i2c_devs(void)
{
    bool found = false;

    for(unsigned int id = 2; id <= 254; id += 2)
    {
        if(I2C2::getInstance().isDevicePresent(id))
        {
            if(id != I2CAddr_AccelerationSensor &&
               id != I2CAddr_LED_Display &&
               id != I2CAddr_TemperatureSensor)
            {
                printf("Found External I2C Device: 0x%02X\n", id);
                found = true;
            }
        }
    }

    return found;
}

static bool hl_init_board_io(void)
{
    bool success = true;

    if(!AS.init()) { puts("ERROR: Acceleration Sensor"); success = false; }
    if(!TS.init()) { puts("ERROR: Temperature Sensor"); success = false; }
    if(!LD.init()) { puts("ERROR: 7-Segment Display"); success = false; }

    /* These devices don't have a way to check if init() failed */
    IS.init(); // IR sensor
    LS.init(); // Light sensor
    LE.init(); // LEDs
    SW.init(); // Switches

    /* Turn off all LEDs */
    LE.setAll(0);

    return success;
}

static void wireless_set_addr_from_file(void)
{
    uint8_t wireless_node_addr = WIRELESS_NODE_ADDR;
    char nAddrStr[8] = { 0 };

    if (FR_OK == Storage::read(WIRELESS_NODE_ADDR_FILE, nAddrStr, sizeof(nAddrStr)-1, 0)) {
        wireless_node_addr = atoi(nAddrStr);
        if (0 == wireless_node_addr || MESH_BROADCAST_ADDR == wireless_node_addr) {
            printf("Invalid node address (%s) specified in %s\n", nAddrStr, WIRELESS_NODE_ADDR_FILE);
        }
        else {
            printf("Wireless node addr set to %i from '%s' file\n",
                    wireless_node_addr, WIRELESS_NODE_ADDR_FILE);
            mesh_set_node_address(wireless_node_addr);
        }
    }
}
