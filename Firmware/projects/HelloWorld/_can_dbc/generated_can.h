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

// static const dbc_msg_hdr_t MASTER_DEBUG_COMMAND_HDR =             {  100, 1 };
// static const dbc_msg_hdr_t MASTER_DEBUG_SIGNALS_HDR =             {  101, 2 };
// static const dbc_msg_hdr_t MASTER_MOTOR_CMD_HDR =                 {  256, 2 };
// static const dbc_msg_hdr_t SENSOR_DATA_HDR =                      {  512, 8 };
// static const dbc_msg_hdr_t GPS_SENSOR_HDR =                       {  768, 2 };
// static const dbc_msg_hdr_t BLUETOOTH_TX_GPS_TARGET_HDR =          {  770, 8 };
// static const dbc_msg_hdr_t GPS_TX_BLUETOOTH_HEADING_HDR =         {  771, 8 };
// static const dbc_msg_hdr_t MOTOR_FEEDBACK_HDR =                   { 1024, 1 };
// static const dbc_msg_hdr_t BLUETOOTH_COMMAND_HDR =                { 1280, 4 };




/// @{ These 'externs' need to be defined in a source file of your project
/// @}


/// Not generating code for dbc_encode_MASTER_DEBUG_COMMAND() since the sender is DBG and we are DRIVER

/// Not generating code for dbc_encode_MASTER_DEBUG_SIGNALS() since the sender is MASTER and we are DRIVER

/// Not generating code for dbc_encode_MASTER_MOTOR_CMD() since the sender is MASTER and we are DRIVER

/// Not generating code for dbc_encode_SENSOR_DATA() since the sender is SENSOR and we are DRIVER

/// Not generating code for dbc_encode_GPS_SENSOR() since the sender is GPS and we are DRIVER

/// Not generating code for dbc_encode_BLUETOOTH_TX_GPS_TARGET() since the sender is BLUETOOTH and we are DRIVER

/// Not generating code for dbc_encode_GPS_TX_BLUETOOTH_HEADING() since the sender is GPS and we are DRIVER

/// Not generating code for dbc_encode_MOTOR_FEEDBACK() since the sender is MOTOR and we are DRIVER

/// Not generating code for dbc_encode_BLUETOOTH_COMMAND() since the sender is BLUETOOTH and we are DRIVER

/// Not generating code for dbc_decode_MASTER_DEBUG_COMMAND() since 'DRIVER' is not the recipient of any of the signals

/// Not generating code for dbc_decode_MASTER_DEBUG_SIGNALS() since 'DRIVER' is not the recipient of any of the signals

/// Not generating code for dbc_decode_MASTER_MOTOR_CMD() since 'DRIVER' is not the recipient of any of the signals

/// Not generating code for dbc_decode_SENSOR_DATA() since 'DRIVER' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GPS_SENSOR() since 'DRIVER' is not the recipient of any of the signals

/// Not generating code for dbc_decode_BLUETOOTH_TX_GPS_TARGET() since 'DRIVER' is not the recipient of any of the signals

/// Not generating code for dbc_decode_GPS_TX_BLUETOOTH_HEADING() since 'DRIVER' is not the recipient of any of the signals

/// Not generating code for dbc_decode_MOTOR_FEEDBACK() since 'DRIVER' is not the recipient of any of the signals

/// Not generating code for dbc_decode_BLUETOOTH_COMMAND() since 'DRIVER' is not the recipient of any of the signals

#endif
