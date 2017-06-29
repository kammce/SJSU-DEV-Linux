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
#include "imu_sensors.hpp"
#include <math.h>
#include "_can_dbc/generated_can.h"
#include "can.h"
#include "gpio.hpp"
#include "lpc_timers.h"
#include "expander.hpp"
#include "ultrasonic.hpp"
#include "eint.h"
#include "adc0.h"

#define DEBUG_IMU               0
#define DEBUG_US                1
#define CAN_UNIT                can1
#define CAN_SPEED               100
#define CAN_RXQ                 10
#define CAN_TXQ                 10
#define PI                      3.14159     /* See M_PI in math.h */
#define HISTORY_DEPTH           1000
#define US_SAMPLES_PER_SECOND   4
#define DEBUG_RANGING           1
#define DECLINATION             13

/* Constants */
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);
const uint32_t PERIOD_DISPATCHER_TASK_STACK_SIZE_BYTES = (512 * 3);

/* Global variables */
float sensor_history[HISTORY_DEPTH][MAX_SENSORS];
int sensor_history_interval[HISTORY_DEPTH];
int history_index = 0;
int pass = 0;
GPIO *gpio_usfen = NULL;
GPIO *gpio_usren = NULL;

/* External variables and functions */
extern void echo_capture_isr(void);
extern LPC_TIM_TypeDef *cap_timer_ptr;

int recalibrate_event_count = 0;

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

bool period_init(void)
{
    /* Print startup message */
    printf("*** Sensor board startup ***\nFILE: %s\nDATE: %s\n", __FILE__, __DATE__);

    /* Set up CAN */
    CAN_init(CAN_UNIT, CAN_SPEED, CAN_RXQ, CAN_TXQ, NULL, NULL);
    CAN_reset_bus(CAN_UNIT);
    CAN_bypass_filter_accept_all_msgs();

    /* Initialize IMU */
    IMU_Sensor::getInstance().init();

    /* Enable ADC input for battery voltage monitor */
    LPC_PINCON->PINSEL1 |= (1 << 20);

    /* Configure P0.30 as USFEN#  */
    gpio_usfen = new GPIO(P0_30);
    gpio_usfen->setAsOutput();
    gpio_usfen->disablePullUpPullDown();
    gpio_usfen->setLow();

    /* Configure P0.29 as USREN#  */
    gpio_usren = new GPIO(P0_29);
    gpio_usren->setAsOutput();
    gpio_usren->disablePullUpPullDown();
    gpio_usren->setLow();

    /* Clear out sensor state */
    for(int i = 0; i < HISTORY_DEPTH; i++)
    {
        for(int j = 0; j < MAX_SENSORS; j++)
        {
            sensor_history[i][j] = 0;
        }
        sensor_history_interval[i] = 0;
    }
    history_index = 0;

    /* Initialize I/O expander */
    expander_init(US_ECHO_INPUT, echo_capture_isr);

    /* Initialize ultrasonic sensor processing */
    ultrasonic_init();

    return true;
}


bool period_reg_tlm(void)
{
    return true;
}


static int interval_timestamp = 0;

void process_ultrasonic_sensors(void)
{
    const float time_per_clock = 1.0f / (float)sys_get_cpu_clock();
    const float distance_unit_divisor = 58.0f;  /* 148=Inches, 58=Centimeters */

    const uint32_t cnt_1sec = sys_get_cpu_clock();   /* 1sec in 48 MHz ticks */
    const uint32_t cnt_50ms = cnt_1sec / 20;         /* 50ms in 48 MHz ticks */

    /* How many values we look back when computing the min (includes the current sample, 4 is n, n-1, n-2, n-3) */
    const int history_range = 4;

    /* Sensor lock-up values */
    const int lock_range = 80;
    const int lock_value = 14;
    const int timeout_ms = 60;
    const int battery_adc_channel = 3;

    uint32_t start_time;
    bool sensor_locked[MAX_SENSORS];
    int scan_count = 0;

    /* Reset timer */
    cap_timer_ptr->TCR |= (1 << 1);
    cap_timer_ptr->TCR &= ~(1 << 1);

    /* Read timer */
    start_time = T1TC;

    /* Turn on ultrasonic sensors (if not already on) */
    gpio_usfen->setLow();
    gpio_usren->setLow();

    /* Start scanning until we don't have enough time left to scan */
    bool scan_running = true;
    while(scan_running)
    {
        /* Convert and send message */
        SENSOR_DATA_m0_t us_sensor_data = {0};
        can_msg_t us_can_msg = {0};

        sensor_response responses[MAX_SENSORS];

        /* Record starting time of scan */
        uint32_t scan_time_start = T1TC;

        /* Scan three sensors */
        ping_ultrasonic_sensors(timeout_ms, responses);

        /* Record stop time of scan */
        uint32_t scan_time_stop = T1TC;

        /* Calculate how much time elapsed during scanning */
        uint32_t scan_time_elapsed = scan_time_stop - scan_time_start;

        /* If less than 50ms elapsed, delay until at least 50ms have elapsed
         * This is because the MB1010 minimum scan time is 50ms so we can't
         * trigger it again. Doing so causes malformed ranging results.
         */
        if(scan_time_elapsed < cnt_50ms)
        {
            /* Determine the count in the future where 50ms have elapsed */
            uint32_t scan_time_end = T1TC + (cnt_50ms - scan_time_elapsed);

            /* Wait for that time to elapse (in coarse 1ms units) */
            while(T1TC < scan_time_end)
            {
                delay_ms(1); /* Coarse delay */
            }
        }

        /* Compute distance for each sensor */
        for(int index = 0; index < MAX_SENSORS; index++)
        {
            float time_ms = ((float)responses[index].pulse_width * time_per_clock) * 1000.0f;
            float distance = (time_ms * 1000.0f) / distance_unit_divisor;
            responses[index].distance = distance;
        }

        /* Update data for each sensor */
        for(int index = 0; index < MAX_SENSORS; index++)
        {
            /* Get last sample */
            float curr = responses[index].distance;

            /* Accumulate into this sample */
            sensor_history[history_index][index] = curr;
        }
        sensor_history_interval[history_index] = interval_timestamp;

        /* Compute minimum sensor value */
        float min[MAX_SENSORS];
        for(int index = 0; index < MAX_SENSORS; index++)
        {
            min[index] = sensor_history[history_index][index];

            for(int history = 0; history < history_range; history++)
            {
                float sample = sensor_history[(history_index - history) % HISTORY_DEPTH][index];
                if(sample < min[index])
                    min[index] = sample;
            }
        }

        /* Assume locked state */
        for(int index = 0; index < MAX_SENSORS; index++)
            sensor_locked[index] = true;

        /* Scan history to disprove sensor lock state */
        for(int history = 0; history < lock_range; history++)
        {
            for(int index = 0; index < MAX_SENSORS; index++)
            {
                if(sensor_locked[index] == true)
                {
                    float sample = sensor_history[(history_index - history) % HISTORY_DEPTH][index];
                    int cooked = (int)sample;
                    if(cooked != lock_value)
                    {
                        sensor_locked[index] = false;
                    }
                }
            }
        }

        /* Determine if any sensors locked */
        bool recalibrate = false;
        for(int index = 0; index < MAX_SENSORS; index++)
        {
            if(sensor_locked[index])
            {
#if DEBUG_US
                printf("*** REQ CALIBRATE %d\n", index);
#endif
                recalibrate = true;
                recalibrate_event_count++;
                LE.set(4, true);
           }
        }

        /* Read battery level */
        uint16_t battery_voltage_level = (adc0_get_reading(battery_adc_channel) >> 2) & 0x3FF;

        if(recalibrate)
        {
            /* Recalibration cycle */
            us_sensor_data.SENSOR_DATA_Sonar_Front_Center = (int)min[1];
            us_sensor_data.SENSOR_DATA_Sonar_Front_Left   = (int)min[2];
            us_sensor_data.SENSOR_DATA_Sonar_Front_Right  = (int)min[0];
            us_sensor_data.SENSOR_DATA_Sonar_Rear_Center  = (int)min[3]; // new
            us_sensor_data.SENSOR_DATA_Sonar_Rear_Left    = 255;
            us_sensor_data.SENSOR_DATA_Sonar_Rear_Right   = battery_voltage_level;

            /* Turn off power */
            gpio_usfen->setHigh();
            gpio_usren->setHigh();

            /* Stop polling */
            scan_running = false;
        }
        else
        {
            /* Normal update */
            us_sensor_data.SENSOR_DATA_Sonar_Front_Center = (int)min[1];
            us_sensor_data.SENSOR_DATA_Sonar_Front_Left   = (int)min[2];
            us_sensor_data.SENSOR_DATA_Sonar_Front_Right  = (int)min[0];
            us_sensor_data.SENSOR_DATA_Sonar_Rear_Center  = (int)min[3]; // new
            us_sensor_data.SENSOR_DATA_Sonar_Rear_Left    = 0;
            us_sensor_data.SENSOR_DATA_Sonar_Rear_Right   = battery_voltage_level;
        }

#if 0
        printf("L:%3.2f C:%3.2f R:%3.2f | B:%3.2f\n", min[2], min[1], min[0], min[3]);
#endif

        /* Format header message */
        dbc_msg_hdr_t us_msg_hdr = dbc_encode_SENSOR_DATA_m0(us_can_msg.data.bytes, &us_sensor_data);
        us_can_msg.msg_id = us_msg_hdr.mid;
        us_can_msg.frame_fields.data_len = us_msg_hdr.dlc;

        /* Send CAN message with sensor data */
        CAN_tx(CAN_UNIT, &us_can_msg, 0);

        /* Bump position in history buffer */
        history_index = (history_index + 1) % HISTORY_DEPTH;

        /* Determine how much scanning time has elapsed */
        uint32_t total_elapsed = T1TC - start_time;
        uint32_t total_remaining = cnt_1sec - total_elapsed;

        /* If there isn't enough time left for a scan (and a bit more), abort */
        if(total_remaining < cnt_50ms * 3)
        {
            scan_running = false;
        }


        ++scan_count;
    } // end while scan
#if DEBUG_US
    printf("%d scans.\n", scan_count);
#endif
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/




//void normalize(double * x, double * y, double * z) {
//    double magnitude = sqrt(*x * *x + *y * *y + *z * *z);
//    *x /= magnitude;
//    *y /= magnitude;
//    *z /= magnitude;
//}

void normalize(double * x, double * y, double * z) {
    *x *= .94f;
    *y *= .94f;
    *z *= .94f;
}

void period_1Hz(uint32_t count)
{
    int total = recalibrate_event_count;
    if(total > 99)
        total = 99;

    /* Display recalibrate event count on 7-seg LED */
    LD.setNumber(total);

    /* Toggle LED2 during a bus-off event */
    if(CAN_is_bus_off(CAN_UNIT))
    {
        CAN_reset_bus(CAN_UNIT);
        LE.toggle(2);
    }

    /* Toggle LED1 to indicate activity */
    LE.toggle(1);

    /* Scan ultrasonic sensors */
    process_ultrasonic_sensors();
}
inline int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void period_10Hz(uint32_t count)
{
    static double filtHx = 0;
    static double filtHy = 0;
    static double filtHz = 0;
    static double filtAx = 0;
    static double filtAy = 0;
    static double filtAz = 0;

    const double DEGTORAD = (180 / PI);
    bool imu_read_successful = IMU_Sensor::getInstance().getXYZ();

    LE.toggle(3);

    if(!imu_read_successful)
    {
        printf("IMU Read ERROR!\n");
        //// TODO: add compass error debug CAN signal to master to disable RC CAR.
    }

#if DEBUG_IMU
   // printf("X: %i Y: %i: Z: %i\n", IMU.Mag_Data.x, IMU.Mag_Data.y, IMU.Mag_Data.z);
#endif

    filtHx = .50 * IMU.Mag_Data.x + .50 * filtHx;
    filtHy = .50 * IMU.Mag_Data.y + .50 * filtHy;
    filtHz = .50 * IMU.Mag_Data.z + .50 * filtHz;

    double normHx = filtHx;
    double normHy = filtHy;
    double normHz = filtHz;

    normalize(&normHx, &normHy, &normHz);

    filtAx = .1 * AS.getX() + .9 * filtAx;
    filtAy = .1 * AS.getY() + .9 * filtAy;
    filtAz = .1 * AS.getZ() + .9 * filtAz;

    double normAx = filtAx;
    double normAy = filtAy;
    double normAz = filtAz;

    normalize(&normAx, &normAy, &normAz);
    double roll = asin(-normAx);
    double pitch = asin(normAy);

    double hy = normHy * cos(roll) + normHx * sin(roll) * sin(pitch) - normHz * cos(pitch)  * sin(roll);
    double hx = normHx * cos(pitch) + normHz * sin(pitch);

    // Calculate heading
    double heading = atan2(normHy, normHx);

    // Convert to degrees
    int32_t headingDegrees = heading * DEGTORAD;

    //double declinationAngle = (4.0 + (26.0 / 60.0)) / DEGTORAD;
    double declinationAngle = (90-15);
    headingDegrees -= declinationAngle;

    // Correct for heading < 0deg and heading > 360deg
    if (headingDegrees < 0)
    {
        headingDegrees += 360;
    }
    if (headingDegrees > 360)
    {
        headingDegrees -= 360;
    }

    const int32_t DEG90 = 20+17;
    const int32_t DEG180 = 74+17;
    const int32_t DEG270 = 300+17;
    const int32_t DEG360 = 343+17;

//    if(headingDegrees <= DEG90)
//    {
//        headingDegrees = map(headingDegrees, 0, DEG90, 0, 45);
//    }
//    else if(headingDegrees <= DEG180 && headingDegrees > DEG90)
//    {
//        headingDegrees = map(headingDegrees, DEG90, DEG180, 45,  90);
//    }
//    else if(headingDegrees <= DEG270 && headingDegrees > DEG180)
//    {
//        headingDegrees = map(headingDegrees, DEG180, DEG270, 90, 270);
//    }
//    else if(headingDegrees <= DEG360 && headingDegrees > DEG270)
//    {
//        headingDegrees = map(headingDegrees, DEG270, DEG360, 180, 270);
//    }

#if DEBUG_IMU
    printf("Heading: %f :: %d\n", heading, headingDegrees);
    printf("x: %f y: %f\n", filtHx, filtHy);
#endif

    uint8_t lightLevel = LS.getPercentValue();
    /*------------------------------------------------------------------------*/
    /* Send IMU data (M1)                                                     */
    /*------------------------------------------------------------------------*/

    SENSOR_DATA_m1_t sensor_data = {0};
    can_msg_t can_msg = {0};

    sensor_data.SENSOR_DATA_Imu_Heading = (int16_t)headingDegrees;
    sensor_data.SENSOR_DATA_Light_Level = lightLevel;

    /* Format header message */
    dbc_msg_hdr_t msg_hdr = dbc_encode_SENSOR_DATA_m1(can_msg.data.bytes, &sensor_data);
    can_msg.msg_id = msg_hdr.mid;
    can_msg.frame_fields.data_len = msg_hdr.dlc;

    /* Send CAN message with sensor data */
    CAN_tx(CAN_UNIT, &can_msg, 0);
}



void period_100Hz(uint32_t count)
{
}



// 1Khz (1ms) is only run if Periodic Dispatcher was configured to run it at main():
// scheduler_add_task(new periodicSchedulerTask(run_1Khz = true));
void period_1000Hz(uint32_t count)
{
}

/* End */
