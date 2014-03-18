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
 * @brief This file provides the configurable parameters for your project.
 */
#ifndef SYSCONFIG_H_
#define SYSCONFIG_H_
#ifdef __cplusplus
extern "C" {
#endif



/** @{ Nordic wireless configuration
 * More settings can be configured at mesh_config.h
 * @warning
 *      The AIR data rate, and channel number must be consistent for your wireless
 *      nodes to talk to each other.
 * @warning
 *      Go to   "PROJECT" --> "Clean"   if you change the settings here.
 */
#define WIRELESS_NODE_ADDR              106    /**< Any value from 1-244 */
#define WIRELESS_CHANNEL_NUM            2499   /**< 2402 - 2500 to avoid collisions among 2+ mesh networks */
#define WIRELESS_AIR_DATARATE_KBPS      2000   /**< Air data rate, can only be 250, 1000, or 2000 kbps */
#define WIRELESS_NODE_NAME             "node"  /**< Wireless node name */
#define WIRELESS_RX_QUEUE_SIZE          3      /**< Number of payloads we can queue */
#define WIRELESS_NODE_ADDR_FILE         "naddr"/**< If this file is present, node address will be set by
                                                     reading ASCII data from this file */
/** @} */


/**
 * Maximum speed to set for SPI #1, which is connected to SD card and SPI Flash.
 * This is the maximum value to set, and actual value may be lower since it
 * will be a power of 2 divider of CPU clock.
 */
#define SPI1_CLOCK_SPEED_MHZ            24
#define SPI0_CLOCK_SPEED_MHZ            8       /***< Nordic wireless requires 1-8Mhz max */
#define I2C2_CLOCK_SPEED_KHZ            100     /***< 100Khz is standard I2C speed */

#define STARTUP_DELAY_MS                2000    /***< Start-up delay in milliseconds */
#define DEL_UPON_LOGFILE_MOVE_TO_SD     0       /***< Delete log file from flash after moving to SD Card */
#define LOG_BOOT_INFO_TO_FILE           0       /***< Log a boot message to "boot.csv" file upon every boot */
#define LOGGER_FILE_NAME                "log"   /***< Logger file name */
#define DISK_TLM_NAME                   "disk"  /***< Filename to save "disk" telemetry variables */
#define MAX_FILES_OPENED                3       /***< Maximum files that can be opened at once */
#define ENABLE_TELEMETRY                0       /***< Enable telemetry system. C_FILE_IO forced enabled if enabled */
#define ENABLE_C_FILE_IO                0       /***< Allow stdio fopen() fclose() to redirect to ff.h */



/**
 * @returns actual System clock as calculated from PLL and Oscillator selection
 * @note The DESIRED_CPU_CLOCK macro defines "Desired" CPU clock, and doesn't guarantee
 *          this clock rate.  This function returns actual CPU clock of the system.
 */
unsigned int sys_get_cpu_clock();


/**
 * Select the clock source:
 * - 0: Internal Clock: 4Mhz  1% Tolerance
 * - 1: External Clock: External Crystal
 * - 2: RTC Clock     : 32.768Khz
 *
 * If the RTC clock is chosen as an input, then sys_clock.cpp will use the closest
 * PLL settings to get you the desired clock rate.  Due to PLL calculations, the
 * RTC PLL setting may delay your startup time (typically 5 seconds) so be patient.
 */
#define CLOCK_SOURCE		0


#define INTERNAL_CLOCK		(4  * 1000 * 1000UL)        ///< Do not change, this is the same on all LPC17XX
#define EXTERNAL_CLOCK      (12 * 1000 * 1000UL)        ///< Change according to your board specification
#define RTC_CLOCK           (32768UL)                   ///< Do not change, this is the typical RTC crystal value

#define DESIRED_CPU_CLOCK	(48 * 1000 * 1000UL)        ///< Define the CPU speed you desire, must be between 1-100Mhz
#define FALLBACK_CPU_CLOCK  (24 * 1000 * 1000UL)        ///< Do not change.  This is the fall-back CPU speed if DESIRED_CPU_CLOCK cannot be attained
#define PERIPHERAL_CLOCK	(DESIRED_CPU_CLOCK / 4UL)   ///< Peripheral clock is set-up by default to be CPU Clock divided by 4
#define TIMER0_US_PER_TICK  (10)                        ///< Microseconds per tick for Timer0 that is used by FreeRTOS run-time stats



/**
 * Set printf & sprintf options - Do a clean build after changing this option
 *  - 0 : Full printf from stdio.h --> Slowest : Uses more code size + RAM
 *  - 1 : iprintf from stdio.h     --> Faster  : -16K Reduced  code size but without floating point support
 *  - 2 : Custom printf            --> Fastest : -21K Smallest code size but not complaint with all printf options
 */
#define USE_REDUCED_PRINTF        0     ///< Configure your printf version here
#define UART0_DEFAULT_RATE_BPS    38400 ///< UART0 is configured at this BPS by Start-up Code - before main()



/**
 * Valid years for RTC.
 * If RTC year is not found to be in between these,
 * RTC will reset to 1/1/2000 00:00:00
 */
#define RTC_VALID_RANGE_YEARS   {2010, 2025}



/**
 * SPI Flash memory is very slow at writing data in general, but can be improved :
 * If this is set to 1, flash pages are erased when a file is deleted such that these pages
 * can be written faster at a later time.
 * If this is set to 0, flash write may take longer, but erasing files will be MUCH quicker.
 *
 * Here is a comparison :
 *  Set to 1 : 700K file written @ 150k/sec, but erased in 25 seconds!!
 *  Set to 0 : 700K file written @  25k/sec, but erased in 50 milliseconds
 *
 * @note _USE_ERASE @ ffconf.h must also be set to 1 to use this feature.
 */
#define SPI_FLASH_OPTIMIZE_WRITE    0

/**
 * The number of SPI Flash memory sectors (512bytes/sector) to reserve.
 * This will not be used by the FAT file system
 */
#define SPI_FLASH_RESERVED_SECTORS    0


/**
 * Do not change anything here.
 * Telemetry needs C file I/O so force enable it
 */
#if (ENABLE_TELEMETRY)
#undef ENABLE_C_FILE_IO
#define ENABLE_C_FILE_IO 1
#endif



#ifdef __cplusplus
}
#endif
#endif /* SYSCONFIG_H_ */
