/// DBC file: lib/_can_dbc/243.dbc    Self node: 'DRIVER'  (ALL = 0)
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

// static const dbc_msg_hdr_t DBC_TEST1_HDR =                        {  500, 8 };
// static const dbc_msg_hdr_t DBC_TEST2_HDR =                        {  501, 8 };
// static const dbc_msg_hdr_t DBC_TEST3_HDR =                        {  502, 8 };
static const dbc_msg_hdr_t DRIVER_HEARTBEAT_HDR =                 {  100, 1 };
static const dbc_msg_hdr_t MOTOR_CMD_HDR =                        {  101, 1 };
static const dbc_msg_hdr_t COUNTER_SEND_HDR =                     {  504, 1 };
static const dbc_msg_hdr_t MOTOR_STATUS_HDR =                     {  400, 3 };
static const dbc_msg_hdr_t SENSOR_SONARS_HDR =                    {  200, 8 };
// static const dbc_msg_hdr_t DBC_TEST4_HDR =                        {  503, 8 };

/// Enumeration(s) for Message: 'DRIVER_HEARTBEAT' from 'DRIVER'
typedef enum {
    DRIVER_HEARTBEAT_cmd_NOOP = 0,
    DRIVER_HEARTBEAT_cmd_SYNC = 1,
    DRIVER_HEARTBEAT_cmd_REBOOT = 2,
} DRIVER_HEARTBEAT_cmd_E ;




/// Message: DRIVER_HEARTBEAT from 'DRIVER', DLC: 1 byte(s), MID: 100
typedef struct {
    DRIVER_HEARTBEAT_cmd_E DRIVER_HEARTBEAT_cmd; ///< B7:0   Destination: SENSOR,MOTOR

    // No dbc_mia_info_t for a message that we will send
} DRIVER_HEARTBEAT_t;


/// Message: MOTOR_CMD from 'DRIVER', DLC: 1 byte(s), MID: 101
typedef struct {
    int8_t MOTOR_CMD_steer : 4;               ///< B3:0  Min: -5 Max: 5   Destination: MOTOR
    uint8_t MOTOR_CMD_drive : 4;              ///< B7:4  Min: 0 Max: 9   Destination: MOTOR

    // No dbc_mia_info_t for a message that we will send
} MOTOR_CMD_t;


/// Message: COUNTER_SEND from 'DRIVER', DLC: 1 byte(s), MID: 504
typedef struct {
    uint8_t COUNTER_SEND_count;               ///< B7:0   Destination: MOTOR

    // No dbc_mia_info_t for a message that we will send
} COUNTER_SEND_t;


/// Message: MOTOR_STATUS from 'DRIVER', DLC: 3 byte(s), MID: 400
typedef struct {
    uint8_t MOTOR_STATUS_wheel_error : 1;     ///< B0:0   Destination: MOTOR,IO
    float MOTOR_STATUS_speed_kph;             ///< B23:8   Destination: MOTOR,IO

    // No dbc_mia_info_t for a message that we will send
} MOTOR_STATUS_t;

/// @{ MUX'd message: SENSOR_SONARS

/// Struct for MUX: m0 (used for transmitting)
typedef struct {
    uint16_t SENSOR_SONARS_err_count;         ///< B15:4   Destination: DRIVER,IO
    float SENSOR_SONARS_left;                 ///< B27:16   Destination: DRIVER,IO
    float SENSOR_SONARS_middle;               ///< B39:28   Destination: DRIVER,IO
    float SENSOR_SONARS_right;                ///< B51:40   Destination: DRIVER,IO
    float SENSOR_SONARS_rear;                 ///< B63:52   Destination: DRIVER,IO

    dbc_mia_info_t mia_info;
} SENSOR_SONARS_m0_t;

/// Struct for MUX: m1 (used for transmitting)
typedef struct {
    uint16_t SENSOR_SONARS_err_count;         ///< B15:4   Destination: DRIVER,IO
    float SENSOR_SONARS_no_filt_left;         ///< B27:16   Destination: DBG
    float SENSOR_SONARS_no_filt_middle;       ///< B39:28   Destination: DBG
    float SENSOR_SONARS_no_filt_right;        ///< B51:40   Destination: DBG
    float SENSOR_SONARS_no_filt_rear;         ///< B63:52   Destination: DBG

    dbc_mia_info_t mia_info;
} SENSOR_SONARS_m1_t;

/// Struct with all the child MUX'd signals (Used for receiving)
typedef struct {
    SENSOR_SONARS_m0_t m0; ///< MUX'd structure
    SENSOR_SONARS_m1_t m1; ///< MUX'd structure
} SENSOR_SONARS_t;
/// @} MUX'd message


/// @{ These 'externs' need to be defined in a source file of your project
extern const uint32_t                             SENSOR_SONARS_m0__MIA_MS;
extern const SENSOR_SONARS_m0_t                   SENSOR_SONARS_m0__MIA_MSG;
extern const uint32_t                             SENSOR_SONARS_m1__MIA_MS;
extern const SENSOR_SONARS_m1_t                   SENSOR_SONARS_m1__MIA_MSG;
/// @}


/// Not generating code for dbc_encode_DBC_TEST1() since the sender is IO and we are DRIVER

/// Not generating code for dbc_encode_DBC_TEST2() since the sender is IO and we are DRIVER

/// Not generating code for dbc_encode_DBC_TEST3() since the sender is IO and we are DRIVER

/// Encode DRIVER's 'DRIVER_HEARTBEAT' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_DRIVER_HEARTBEAT(uint8_t bytes[8], DRIVER_HEARTBEAT_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    raw = ((uint32_t)(((from->DRIVER_HEARTBEAT_cmd)))) & 0xff;
    bytes[0] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B0

    return DRIVER_HEARTBEAT_HDR;
}

/// Encode and send for dbc_encode_DRIVER_HEARTBEAT() message
static inline bool dbc_encode_and_send_DRIVER_HEARTBEAT(DRIVER_HEARTBEAT_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_DRIVER_HEARTBEAT(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode DRIVER's 'MOTOR_CMD' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_MOTOR_CMD(uint8_t bytes[8], MOTOR_CMD_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    if(from->MOTOR_CMD_steer < -5) { from->MOTOR_CMD_steer = -5; } // Min value: -5
    if(from->MOTOR_CMD_steer > 5) { from->MOTOR_CMD_steer = 5; } // Max value: 5
    raw = ((uint32_t)(((from->MOTOR_CMD_steer - (-5))))) & 0x0f;
    bytes[0] |= (((uint8_t)(raw) & 0x0f)); ///< 4 bit(s) starting from B0

    // Not doing min value check since the signal is unsigned already
    if(from->MOTOR_CMD_drive > 9) { from->MOTOR_CMD_drive = 9; } // Max value: 9
    raw = ((uint32_t)(((from->MOTOR_CMD_drive)))) & 0x0f;
    bytes[0] |= (((uint8_t)(raw) & 0x0f) << 4); ///< 4 bit(s) starting from B4

    return MOTOR_CMD_HDR;
}

/// Encode and send for dbc_encode_MOTOR_CMD() message
static inline bool dbc_encode_and_send_MOTOR_CMD(MOTOR_CMD_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_MOTOR_CMD(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode DRIVER's 'COUNTER_SEND' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_COUNTER_SEND(uint8_t bytes[8], COUNTER_SEND_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    raw = ((uint32_t)(((from->COUNTER_SEND_count)))) & 0xff;
    bytes[0] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B0

    return COUNTER_SEND_HDR;
}

/// Encode and send for dbc_encode_COUNTER_SEND() message
static inline bool dbc_encode_and_send_COUNTER_SEND(COUNTER_SEND_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_COUNTER_SEND(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Encode DRIVER's 'MOTOR_STATUS' message
/// @returns the message header of this message
static inline dbc_msg_hdr_t dbc_encode_MOTOR_STATUS(uint8_t bytes[8], MOTOR_STATUS_t *from)
{
    uint32_t raw;
    bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=bytes[5]=bytes[6]=bytes[7]=0;

    raw = ((uint32_t)(((from->MOTOR_STATUS_wheel_error)))) & 0x01;
    bytes[0] |= (((uint8_t)(raw) & 0x01)); ///< 1 bit(s) starting from B0

    raw = ((uint32_t)(((from->MOTOR_STATUS_speed_kph) / 0.001) + 0.5)) & 0xffff;
    bytes[1] |= (((uint8_t)(raw) & 0xff)); ///< 8 bit(s) starting from B8
    bytes[2] |= (((uint8_t)(raw >> 8) & 0xff)); ///< 8 bit(s) starting from B16

    return MOTOR_STATUS_HDR;
}

/// Encode and send for dbc_encode_MOTOR_STATUS() message
static inline bool dbc_encode_and_send_MOTOR_STATUS(MOTOR_STATUS_t *from)
{
    uint8_t bytes[8];
    const dbc_msg_hdr_t hdr = dbc_encode_MOTOR_STATUS(bytes, from);
    return dbc_app_send_can_msg(hdr.mid, hdr.dlc, bytes);
}



/// Not generating code for dbc_encode_SENSOR_SONARS() since the sender is SENSOR and we are DRIVER

/// Not generating code for dbc_encode_DBC_TEST4() since the sender is IO and we are DRIVER

/// Not generating code for dbc_decode_DBC_TEST1() since 'DRIVER' is not the recipient of any of the signals

/// Not generating code for dbc_decode_DBC_TEST2() since 'DRIVER' is not the recipient of any of the signals

/// Not generating code for dbc_decode_DBC_TEST3() since 'DRIVER' is not the recipient of any of the signals

/// Not generating code for dbc_decode_DRIVER_HEARTBEAT() since 'DRIVER' is not the recipient of any of the signals

/// Not generating code for dbc_decode_MOTOR_CMD() since 'DRIVER' is not the recipient of any of the signals

/// Not generating code for dbc_decode_COUNTER_SEND() since 'DRIVER' is not the recipient of any of the signals

/// Not generating code for dbc_decode_MOTOR_STATUS() since 'DRIVER' is not the recipient of any of the signals

/// Decode SENSOR's 'SENSOR_SONARS' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool dbc_decode_SENSOR_SONARS(SENSOR_SONARS_t *to, const uint8_t bytes[8], const dbc_msg_hdr_t *hdr)
{
    const bool success = true;
    // If msg header is provided, check if the DLC and the MID match
    if (NULL != hdr && (hdr->dlc != SENSOR_SONARS_HDR.dlc || hdr->mid != SENSOR_SONARS_HDR.mid)) {
        return !success;
    }

    uint32_t raw;
    // Decode the MUX
    raw  = ((uint32_t)((bytes[0]) & 0x0f)); ///< 4 bit(s) from B0
    const uint8_t MUX = ((raw));

    if (0 == MUX) {
        // Non Muxed signals (part of all MUX'd structures)
        raw  = ((uint32_t)((bytes[0] >> 4) & 0x0f)); ///< 4 bit(s) from B4
        raw |= ((uint32_t)((bytes[1]))) << 4; ///< 8 bit(s) from B8
        to->m0.SENSOR_SONARS_err_count = ((raw));

        raw  = ((uint32_t)((bytes[2]))); ///< 8 bit(s) from B16
        raw |= ((uint32_t)((bytes[3]) & 0x0f)) << 8; ///< 4 bit(s) from B24
        to->m0.SENSOR_SONARS_left = ((raw * 0.1));
        raw  = ((uint32_t)((bytes[3] >> 4) & 0x0f)); ///< 4 bit(s) from B28
        raw |= ((uint32_t)((bytes[4]))) << 4; ///< 8 bit(s) from B32
        to->m0.SENSOR_SONARS_middle = ((raw * 0.1));
        raw  = ((uint32_t)((bytes[5]))); ///< 8 bit(s) from B40
        raw |= ((uint32_t)((bytes[6]) & 0x0f)) << 8; ///< 4 bit(s) from B48
        to->m0.SENSOR_SONARS_right = ((raw * 0.1));
        raw  = ((uint32_t)((bytes[6] >> 4) & 0x0f)); ///< 4 bit(s) from B52
        raw |= ((uint32_t)((bytes[7]))) << 4; ///< 8 bit(s) from B56
        to->m0.SENSOR_SONARS_rear = ((raw * 0.1));

        to->m0.mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    }
    else if (1 == MUX) {
        // Non Muxed signals (part of all MUX'd structures)
        raw  = ((uint32_t)((bytes[0] >> 4) & 0x0f)); ///< 4 bit(s) from B4
        raw |= ((uint32_t)((bytes[1]))) << 4; ///< 8 bit(s) from B8
        to->m1.SENSOR_SONARS_err_count = ((raw));

        raw  = ((uint32_t)((bytes[2]))); ///< 8 bit(s) from B16
        raw |= ((uint32_t)((bytes[3]) & 0x0f)) << 8; ///< 4 bit(s) from B24
        to->m1.SENSOR_SONARS_no_filt_left = ((raw * 0.1));
        raw  = ((uint32_t)((bytes[3] >> 4) & 0x0f)); ///< 4 bit(s) from B28
        raw |= ((uint32_t)((bytes[4]))) << 4; ///< 8 bit(s) from B32
        to->m1.SENSOR_SONARS_no_filt_middle = ((raw * 0.1));
        raw  = ((uint32_t)((bytes[5]))); ///< 8 bit(s) from B40
        raw |= ((uint32_t)((bytes[6]) & 0x0f)) << 8; ///< 4 bit(s) from B48
        to->m1.SENSOR_SONARS_no_filt_right = ((raw * 0.1));
        raw  = ((uint32_t)((bytes[6] >> 4) & 0x0f)); ///< 4 bit(s) from B52
        raw |= ((uint32_t)((bytes[7]))) << 4; ///< 8 bit(s) from B56
        to->m1.SENSOR_SONARS_no_filt_rear = ((raw * 0.1));

        to->m1.mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    }
    else {
        return !success;
    }

    return success;
}


/// Not generating code for dbc_decode_DBC_TEST4() since 'DRIVER' is not the recipient of any of the signals

/// Handle the MIA for SENSOR's SENSOR_SONARS for MUX "m0" message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_SENSOR_SONARS_m0(SENSOR_SONARS_m0_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= SENSOR_SONARS_m0__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
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
/// @post    If the MIA counter reaches the MIA threshold, MIA struct will be copied to *msg
static inline bool dbc_handle_mia_SENSOR_SONARS_m1(SENSOR_SONARS_m1_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const dbc_mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= SENSOR_SONARS_m1__MIA_MS);

    if (!msg->mia_info.is_mia) { // Not MIA yet, so keep incrementing the MIA counter
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { // Previously not MIA, but it is MIA now
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = SENSOR_SONARS_m1__MIA_MSG;
        msg->mia_info.mia_counter_ms = SENSOR_SONARS_m1__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

#endif
