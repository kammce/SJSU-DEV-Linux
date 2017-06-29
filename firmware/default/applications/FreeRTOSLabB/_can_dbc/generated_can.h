/// DBC file: lib/_can_dbc/243.dbc    Self node: 'DBG'  (ALL = 0)
/// This file can be included by a source file, for example: #include "generated.h"
#ifndef __GENEARTED_DBC_PARSER
#define __GENERATED_DBC_PARSER
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>



/// Extern function needed for dbc_encode_and_send()
extern bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8]);

/// Missing in Action structure
typedef struct {
    uint32_t is_mia : 1;          ///< Missing in action flag
    uint32_t mia_counter_ms : 31; ///< Missing in action counter
} dbc_mia_info_t;

/// CAN message header structure
typedef struct { 
    uint32_t mid; ///< Message ID of the message
    uint8_t  dlc; ///< Data length of the message
} dbc_msg_hdr_t; 

static const dbc_msg_hdr_t MASTER_DEBUG_COMMAND_HDR =             {  100, 1 };
static const dbc_msg_hdr_t MASTER_DEBUG_SIGNALS_HDR =             {  101, 2 };
// static const dbc_msg_hdr_t MASTER_MOTOR_CMD_HDR =                 {  256, 2 };
// static const dbc_msg_hdr_t SENSOR_DATA_HDR =                      {  512, 8 };
// static const dbc_msg_hdr_t GPS_SENSOR_HDR =                       {  768, 2 };
// static const dbc_msg_hdr_t BLUETOOTH_TX_GPS_TARGET_HDR =          {  770, 8 };
// static const dbc_msg_hdr_t GPS_TX_BLUETOOTH_HEADING_HDR =         {  771, 8 };
static const dbc_msg_hdr_t MOTOR_FEEDBACK_HDR =                   { 1024, 1 };
// static const dbc_msg_hdr_t BLUETOOTH_COMMAND_HDR =                { 1280, 4 };




/// Message: MASTER_DEBUG_COMMAND from 'DBG', DLC: 1 byte(s), MID: 100
typedef struct {
    uint8_t MASTER_CAR_ENABLE : 1;            ///< B0:0  Min: 0 Max: 1   Destination: MASTER

    // No dbc_mia_info_t for a message that we will send
} MASTER_DEBUG_COMMAND_t;


/// Message: MASTER_DEBUG_SIGNALS from 'MASTER', DLC: 2 byte(s), MID: 101
typedef struct {
    uint8_t OBSTACLE_MODE_DEBUG : 1;          ///< B0:0  Min: 0 Max: 1   Destination: DBG
    uint8_t PI_MODE_DEBUG : 1;                ///< B1:1  Min: 0 Max: 1   Destination: DBG
    uint8_t SENSOR_RIGHT_TRIGGER : 1;         ///< B2:2  Min: 0 Max: 1   Destination: DBG
    uint8_t SENSOR_LEFT_TRIGGER : 1;          ///< B3:3  Min: 0 Max: 1   Destination: DBG
    uint8_t SENSOR_MIDDLE_TRIGGER : 1;        ///< B4:4  Min: 0 Max: 1   Destination: DBG
    uint8_t MASTER_ENABLED_DISABLED_DEBUG : 1; ///< B5:5  Min: 0 Max: 1   Destination: DBG
    int8_t OBSTACLE_SPEED_DEBUG : 4;          ///< B9:6  Min: -5 Max: 5   Destination: DBG
    uint8_t PI_SPEED_DEBUG : 4;               ///< B14:11  Min: 0 Max: 9   Destination: DBG

    dbc_mia_info_t mia_info;
} MASTER_DEBUG_SIGNALS_t;


/// Message: MOTOR_FEEDBACK from 'MOTOR', DLC: 1 byte(s), MID: 1024
typedef struct {
    int8_t MOTOR_FEEDBACK_Tachometer;         ///< B7:0  Min: -100 Max: 100   Destination: BLUETOOTH,DBG

    dbc_mia_info_t mia_info;
} MOTOR_FEEDBACK_t;


/// @{ These 'externs' need to be defined in a source file of your project
extern const uint32_t                             MASTER_DEBUG_SIGNALS__MIA_MS;
extern const MASTER_DEBUG_SIGNALS_t               MASTER_DEBUG_SIGNALS__MIA_MSG;
extern const uint32_t                             MOTOR_FEEDBACK__MIA_MS;
extern const MOTOR_FEEDBACK_t                     MOTOR_FEEDBACK__MIA_MSG;
/// @}


/// Encode DBG's 'MASTER_DEBUG_COMMAND' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_MASTER_DEBUG_COMMAND(uint8_t bytes[8], MASTER_DEBUG_COMMAND_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    // Not doing min value check since the signal is unsigned already
    if(from->MASTER_CAR_ENABLE > 1) { from->MASTER_CAR_ENABLE = 1; } // Max value: 1
    raw = ((uint32_t)(((from->MASTER_CAR_ENABLE)))) & 0x01;
    bytes[0] |= (((uint8_t)(raw) & 0x01)); ///< 1 bit(s) starting from B0

    return MASTER_DEBUG_COMMAND_HDR;
}

/// Encode and send for dbc_encode_MASTER_DEBUG_COMMAND() message
static inline bool dbc_encode_and_send_MASTER_DEBUG_COMMAND(MASTER_DEBUG_COMMAND_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_MASTER_DEBUG_COMMAND(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Not generating code for dbc_encode_MASTER_DEBUG_SIGNALS() since the sender is MASTER and we are DBG

/// Not generating code for dbc_encode_MASTER_MOTOR_CMD() since the sender is MASTER and we are DBG

/// Not generating code for dbc_encode_SENSOR_DATA() since the sender is SENSOR and we are DBG

/// Not generating code for dbc_encode_GPS_SENSOR() since the sender is GPS and we are DBG

/// Not generating code for dbc_encode_BLUETOOTH_TX_GPS_TARGET() since the sender is BLUETOOTH and we are DBG

/// Not generating code for dbc_encode_GPS_TX_BLUETOOTH_HEADING() since the sender is GPS and we are DBG

/// Not generating code for dbc_encode_MOTOR_FEEDBACK() since the sender is MOTOR and we are DBG

/// Not generating code for dbc_encode_BLUETOOTH_COMMAND() since the sender is BLUETOOTH and we are DBG

/// Not generating code for dbc_decode_MASTER_DEBUG_COMMAND() since 'DBG' is not the recipient of any of the signals

/// Decode MASTER's 'MASTER_DEBUG_SIGNALS' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_MASTER_DEBUG_SIGNALS(MASTER_DEBUG_SIGNALS_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != MASTER_DEBUG_SIGNALS_HDR.dlc || hdr->mid != MASTER_DEBUG_SIGNALS_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    raw  = ((uint32_t)((bytes[0]) & 0x01)); ///< 1 bit(s) from B0
    to->OBSTACLE_MODE_DEBUG = ((raw));
    raw  = ((uint32_t)((bytes[0] >> 1) & 0x01)); ///< 1 bit(s) from B1
    to->PI_MODE_DEBUG = ((raw));
    raw  = ((uint32_t)((bytes[0] >> 2) & 0x01)); ///< 1 bit(s) from B2
    to->SENSOR_RIGHT_TRIGGER = ((raw));
    raw  = ((uint32_t)((bytes[0] >> 3) & 0x01)); ///< 1 bit(s) from B3
    to->SENSOR_LEFT_TRIGGER = ((raw));
    raw  = ((uint32_t)((bytes[0] >> 4) & 0x01)); ///< 1 bit(s) from B4
    to->SENSOR_MIDDLE_TRIGGER = ((raw));
    raw  = ((uint32_t)((bytes[0] >> 5) & 0x01)); ///< 1 bit(s) from B5
    to->MASTER_ENABLED_DISABLED_DEBUG = ((raw));
    raw  = ((uint32_t)((bytes[0] >> 6) & 0x03)); ///< 2 bit(s) from B6
    raw |= ((uint32_t)((bytes[1]) & 0x03)) << 2; ///< 2 bit(s) from B8
    to->OBSTACLE_SPEED_DEBUG = ((raw) + (-5));
    raw  = ((uint32_t)((bytes[1] >> 3) & 0x0f)); ///< 4 bit(s) from B11
    to->PI_SPEED_DEBUG = ((raw));

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Not generating code for dbc_decode_MASTER_MOTOR_CMD() since 'DBG' is not the recipient of any of the signals

/// Not generating code for dbc_decode_SENSOR_DATA() since 'DBG' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GPS_SENSOR() since 'DBG' is not the recipient of any of the signals

/// Not generating code for dbc_decode_BLUETOOTH_TX_GPS_TARGET() since 'DBG' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GPS_TX_BLUETOOTH_HEADING() since 'DBG' is not the recipient of any of the signals

/// Decode MOTOR's 'MOTOR_FEEDBACK' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_MOTOR_FEEDBACK(MOTOR_FEEDBACK_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != MOTOR_FEEDBACK_HDR.dlc || hdr->mid != MOTOR_FEEDBACK_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    raw  = ((uint32_t)((bytes[0]))); ///< 8 bit(s) from B0
    to->MOTOR_FEEDBACK_Tachometer = ((raw) + (-100));

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter

    return success;
}


/// Not generating code for dbc_decode_BLUETOOTH_COMMAND() since 'DBG' is not the recipient of any of the signals

/// Handle the MIA for MASTER's MASTER_DEBUG_SIGNALS message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_MASTER_DEBUG_SIGNALS(MASTER_DEBUG_SIGNALS_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= MASTER_DEBUG_SIGNALS__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = MASTER_DEBUG_SIGNALS__MIA_MSG;
        msg->mia_info.mia_counter_ms = MASTER_DEBUG_SIGNALS__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for MOTOR's MOTOR_FEEDBACK message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_MOTOR_FEEDBACK(MOTOR_FEEDBACK_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= MOTOR_FEEDBACK__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = MOTOR_FEEDBACK__MIA_MSG;
        msg->mia_info.mia_counter_ms = MOTOR_FEEDBACK__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

#endif
