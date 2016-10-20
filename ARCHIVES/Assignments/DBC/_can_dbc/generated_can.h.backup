/// DBC file: ../_can_dbc/243.dbc    Self node: 'DRIVER'
/// This file can be included by a source file, for example: #include "generated.h"
#ifndef __GENEARTED_DBC_PARSER
#define __GENERATED_DBC_PARSER
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>



/// Missing in Action structure
typedef struct {
    uint32_t is_mia : 1;          ///< Missing in action flag
    uint32_t mia_counter_ms : 31; ///< Missing in action counter
} mia_info_t;

/// CAN message header structure
typedef struct { 
    uint32_t mid; ///< Message ID of the message
    uint8_t  dlc; ///< Data length of the message
} msg_hdr_t; 

static const msg_hdr_t DRIVER_TX_DRIVER_HEARTBEAT_HDR =       {  100, 1 };
static const msg_hdr_t SENSOR_TX_SENSOR_SONARS_HDR =          {  200, 8 };
static const msg_hdr_t DRIVER_TX_MOTOR_CMD_HDR =              {  300, 1 };
static const msg_hdr_t MOTOR_TX_MOTOR_STATUS_HDR =            {  400, 3 };
// static const msg_hdr_t IO_TX_IO_DEBUG_HDR =                   {  901, 1 };

/// Enumeration for Message: 'DRIVER_HEARTBEAT' from 'DRIVER'
typedef enum {
    DRIVER_HEARTBEAT_cmd_SYNC = 1,
    DRIVER_HEARTBEAT_cmd_REBOOT = 2,
    DRIVER_HEARTBEAT_cmd_NOOP = 0,
} DRIVER_HEARTBEAT_cmd_E ;



/// Message: DRIVER_HEARTBEAT from 'DRIVER', DLC: 1 byte(s), MID: 100
typedef struct {
    DRIVER_HEARTBEAT_cmd_E DRIVER_HEARTBEAT_cmd; ///< B7:0   Destination: SENSOR,MOTOR

    mia_info_t mia_info;
} DRIVER_TX_DRIVER_HEARTBEAT_t;

/// @{ MUX'd message: SENSOR_SONARS

/// Struct for MUX: m0
typedef struct {
    uint16_t SENSOR_SONARS_err_count;         ///< B15:4   Destination: DRIVER,IO
    float SENSOR_SONARS_left;                 ///< B27:16   Destination: DRIVER,IO
    float SENSOR_SONARS_middle;               ///< B39:28   Destination: DRIVER,IO
    float SENSOR_SONARS_right;                ///< B51:40   Destination: DRIVER,IO
    float SENSOR_SONARS_rear;                 ///< B63:52   Destination: DRIVER,IO

    mia_info_t mia_info;
} SENSOR_TX_SENSOR_SONARS_m0_t;

/// Struct for MUX: m1
typedef struct {
    uint16_t SENSOR_SONARS_err_count;         ///< B15:4   Destination: DRIVER,IO
    float SENSOR_SONARS_no_filt_left;         ///< B27:16   Destination: DBG
    float SENSOR_SONARS_no_filt_middle;       ///< B39:28   Destination: DBG
    float SENSOR_SONARS_no_filt_right;        ///< B51:40   Destination: DBG
    float SENSOR_SONARS_no_filt_rear;         ///< B63:52   Destination: DBG

    mia_info_t mia_info;
} SENSOR_TX_SENSOR_SONARS_m1_t;

/// Struct with all the child MUX'd signals
typedef struct {
    SENSOR_TX_SENSOR_SONARS_m0_t m0; ///< MUX'd structure
    SENSOR_TX_SENSOR_SONARS_m1_t m1; ///< MUX'd structure
} SENSOR_TX_SENSOR_SONARS_t;
/// @} MUX'd message


/// Message: MOTOR_CMD from 'DRIVER', DLC: 1 byte(s), MID: 300
typedef struct {
    int8_t MOTOR_CMD_steer : 4;               ///< B3:0  Min: -5 Max: 5   Destination: MOTOR
    uint8_t MOTOR_CMD_drive : 4;              ///< B7:4  Min: 0 Max: 9   Destination: MOTOR

    mia_info_t mia_info;
} DRIVER_TX_MOTOR_CMD_t;


/// Message: MOTOR_STATUS from 'MOTOR', DLC: 3 byte(s), MID: 400
typedef struct {
    uint8_t MOTOR_STATUS_wheel_error : 1;     ///< B0:0   Destination: DRIVER
    float MOTOR_STATUS_speed_kph;             ///< B23:8   Destination: DRIVER

    mia_info_t mia_info;
} MOTOR_TX_MOTOR_STATUS_t;


/// These 'externs' need to be defined in a source file of your project
extern const uint32_t                             SENSOR_SONARS_m0__MIA_MS;
extern const SENSOR_TX_SENSOR_SONARS_m0_t         SENSOR_SONARS_m0__MIA_MSG;
extern const uint32_t                             SENSOR_SONARS_m1__MIA_MS;
extern const SENSOR_TX_SENSOR_SONARS_m1_t         SENSOR_SONARS_m1__MIA_MSG;
extern const uint32_t                             MOTOR_STATUS__MIA_MS;
extern const MOTOR_TX_MOTOR_STATUS_t              MOTOR_STATUS__MIA_MSG;

/// Encode DRIVER's 'DRIVER_HEARTBEAT' message
/// @returns the message header of this message
static msg_hdr_t DRIVER_TX_DRIVER_HEARTBEAT_encode(uint64_t *to, DRIVER_TX_DRIVER_HEARTBEAT_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint64_t raw;
    uint8_t *bytes = (uint8_t*) to;

    raw = ((uint64_t)(((from->DRIVER_HEARTBEAT_cmd - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw >> 0) & 0xff) << 0); ///< 8 bit(s) starting from B0

    return DRIVER_TX_DRIVER_HEARTBEAT_HDR;
}


/// Not generating code for SENSOR_TX_SENSOR_SONARS_encode() since the sender is SENSOR and we are DRIVER

/// Encode DRIVER's 'MOTOR_CMD' message
/// @returns the message header of this message
static msg_hdr_t DRIVER_TX_MOTOR_CMD_encode(uint64_t *to, DRIVER_TX_MOTOR_CMD_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint64_t raw;
    uint8_t *bytes = (uint8_t*) to;

    if(from->MOTOR_CMD_steer < -5) { from->MOTOR_CMD_steer = -5; }
    if(from->MOTOR_CMD_steer > 5) { from->MOTOR_CMD_steer = 5; }
    raw = ((uint64_t)(((from->MOTOR_CMD_steer - (-5)) / 1.0) + 0.5)) & 0x0f;
    bytes[0] |= (((uint8_t)(raw >> 0) & 0x0f) << 0); ///< 4 bit(s) starting from B0

    if(from->MOTOR_CMD_drive < 0) { from->MOTOR_CMD_drive = 0; }
    if(from->MOTOR_CMD_drive > 9) { from->MOTOR_CMD_drive = 9; }
    raw = ((uint64_t)(((from->MOTOR_CMD_drive - (0)) / 1.0) + 0.5)) & 0x0f;
    bytes[0] |= (((uint8_t)(raw >> 0) & 0x0f) << 4); ///< 4 bit(s) starting from B4

    return DRIVER_TX_MOTOR_CMD_HDR;
}


/// Not generating code for MOTOR_TX_MOTOR_STATUS_encode() since the sender is MOTOR and we are DRIVER

/// Not generating code for IO_TX_IO_DEBUG_encode() since the sender is IO and we are DRIVER

/// Not generating code for DRIVER_TX_DRIVER_HEARTBEAT_decode() since 'DRIVER' is not the recipient of any of the signals

/// Decode SENSOR's 'SENSOR_SONARS' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool SENSOR_TX_SENSOR_SONARS_decode(SENSOR_TX_SENSOR_SONARS_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != SENSOR_TX_SENSOR_SONARS_HDR.dlc || hdr->mid != SENSOR_TX_SENSOR_SONARS_HDR.mid)) {
        return !success;
    }
    uint64_t raw;
    const uint8_t *bytes = (const uint8_t*) from;

    // Decode the MUX
    raw  = ((uint64_t)((bytes[0] >> 0) & 0x0f)) << 0; ///< 4 bit(s) from B0
    const uint8_t MUX =  ((raw * 1.0) + (0));

    if (0 == MUX) {
        // Non Muxed signals (part of all MUX'd structures)
        raw  = ((uint64_t)((bytes[0] >> 4) & 0x0f)) << 0; ///< 4 bit(s) from B4
        raw |= ((uint64_t)((bytes[1] >> 0) & 0xff)) << 4; ///< 8 bit(s) from B8
        to->m0.SENSOR_SONARS_err_count =  ((raw * 1.0) + (0));

        raw  = ((uint64_t)((bytes[2] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B16
        raw |= ((uint64_t)((bytes[3] >> 0) & 0x0f)) << 8; ///< 4 bit(s) from B24
        to->m0.SENSOR_SONARS_left =  ((raw * 0.1) + (0));
        raw  = ((uint64_t)((bytes[3] >> 4) & 0x0f)) << 0; ///< 4 bit(s) from B28
        raw |= ((uint64_t)((bytes[4] >> 0) & 0xff)) << 4; ///< 8 bit(s) from B32
        to->m0.SENSOR_SONARS_middle =  ((raw * 0.1) + (0));
        raw  = ((uint64_t)((bytes[5] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B40
        raw |= ((uint64_t)((bytes[6] >> 0) & 0x0f)) << 8; ///< 4 bit(s) from B48
        to->m0.SENSOR_SONARS_right =  ((raw * 0.1) + (0));
        raw  = ((uint64_t)((bytes[6] >> 4) & 0x0f)) << 0; ///< 4 bit(s) from B52
        raw |= ((uint64_t)((bytes[7] >> 0) & 0xff)) << 4; ///< 8 bit(s) from B56
        to->m0.SENSOR_SONARS_rear =  ((raw * 0.1) + (0));

        to->m0.mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    }
    else if (1 == MUX) {
        // Non Muxed signals (part of all MUX'd structures)
        raw  = ((uint64_t)((bytes[0] >> 4) & 0x0f)) << 0; ///< 4 bit(s) from B4
        raw |= ((uint64_t)((bytes[1] >> 0) & 0xff)) << 4; ///< 8 bit(s) from B8
        to->m1.SENSOR_SONARS_err_count =  ((raw * 1.0) + (0));

        raw  = ((uint64_t)((bytes[2] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B16
        raw |= ((uint64_t)((bytes[3] >> 0) & 0x0f)) << 8; ///< 4 bit(s) from B24
        to->m1.SENSOR_SONARS_no_filt_left =  ((raw * 0.1) + (0));
        raw  = ((uint64_t)((bytes[3] >> 4) & 0x0f)) << 0; ///< 4 bit(s) from B28
        raw |= ((uint64_t)((bytes[4] >> 0) & 0xff)) << 4; ///< 8 bit(s) from B32
        to->m1.SENSOR_SONARS_no_filt_middle =  ((raw * 0.1) + (0));
        raw  = ((uint64_t)((bytes[5] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B40
        raw |= ((uint64_t)((bytes[6] >> 0) & 0x0f)) << 8; ///< 4 bit(s) from B48
        to->m1.SENSOR_SONARS_no_filt_right =  ((raw * 0.1) + (0));
        raw  = ((uint64_t)((bytes[6] >> 4) & 0x0f)) << 0; ///< 4 bit(s) from B52
        raw |= ((uint64_t)((bytes[7] >> 0) & 0xff)) << 4; ///< 8 bit(s) from B56
        to->m1.SENSOR_SONARS_no_filt_rear =  ((raw * 0.1) + (0));

        to->m1.mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    }
    else {
        return !success;
    }

    return success;
}


/// Not generating code for DRIVER_TX_MOTOR_CMD_decode() since 'DRIVER' is not the recipient of any of the signals

/// Decode MOTOR's 'MOTOR_STATUS' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool MOTOR_TX_MOTOR_STATUS_decode(MOTOR_TX_MOTOR_STATUS_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != MOTOR_TX_MOTOR_STATUS_HDR.dlc || hdr->mid != MOTOR_TX_MOTOR_STATUS_HDR.mid)) {
        return !success;
    }
    uint64_t raw;
    const uint8_t *bytes = (const uint8_t*) from;

    raw  = ((uint64_t)((bytes[0] >> 0) & 0x01)) << 0; ///< 1 bit(s) from B0
    to->MOTOR_STATUS_wheel_error =  ((raw * 1.0) + (0));
    raw  = ((uint64_t)((bytes[1] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B8
    raw |= ((uint64_t)((bytes[2] >> 0) & 0xff)) << 8; ///< 8 bit(s) from B16
    to->MOTOR_STATUS_speed_kph =  ((raw * 0.001) + (0));

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Not generating code for IO_TX_IO_DEBUG_decode() since 'DRIVER' is not the recipient of any of the signals

/// Handle the MIA for SENSOR's SENSOR_SONARS for MUX "m0" message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool SENSOR_TX_SENSOR_SONARS_m0_handle_mia(SENSOR_TX_SENSOR_SONARS_m0_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= SENSOR_SONARS_m0__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = SENSOR_SONARS_m0__MIA_MSG;
        msg->mia_info.mia_counter_ms = SENSOR_SONARS_m0__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for SENSOR's SENSOR_SONARS for MUX "m1" message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool SENSOR_TX_SENSOR_SONARS_m1_handle_mia(SENSOR_TX_SENSOR_SONARS_m1_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= SENSOR_SONARS_m1__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = SENSOR_SONARS_m1__MIA_MSG;
        msg->mia_info.mia_counter_ms = SENSOR_SONARS_m1__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for MOTOR's MOTOR_STATUS message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool MOTOR_TX_MOTOR_STATUS_handle_mia(MOTOR_TX_MOTOR_STATUS_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= MOTOR_STATUS__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = MOTOR_STATUS__MIA_MSG;
        msg->mia_info.mia_counter_ms = MOTOR_STATUS__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

#endif
