/*
 
 */

#ifndef L5_APPLICATION_ULTRASONIC_HPP_
#define L5_APPLICATION_ULTRASONIC_HPP_

#define MAX_SENSORS                     4

#define US_TRIG_PULSE_WIDTH             25          /* us (MB1010 DS says 20us min) */
#define US_TRIG_LEVEL_ON                1           /* High level */
#define US_TRIG_LEVEL_OFF               0           /* Low level */

#define ACTIVE_HIGH(x)                  (x)
#define ACTIVE_LOW(x)                   (!(x))

#define US_ECHO_INPUT                   IF_DOUT         // read test pin p2.6 green lead
#define US_ECHO_ACTIVE(x)               ACTIVE_HIGH(x)   //

enum result_type
{
    US_RESULT_TIMEOUT,
    US_RESULT_NORESPONSE,
    US_RESULT_NORMAL,
};

enum scan_states
{
    ST_IDLE,        /* No request to read sensors */
    ST_REQUEST,     /* Request has been made */
    ST_MEASURE      /* Start of pulse has been detected */
};

typedef struct
{
    uint32_t pulse_width;
    float duration;
    float distance;
    scan_states state;
    result_type type;
} sensor_response;


enum ultrasonic_sensor_identifiers
{
    US_FRONT_LEFT,
    US_FRONT_CENTER,
    US_FRONT_RIGHT,
    US_BACK_LEFT,
    US_BACK_CENTER,
    US_BACK_RIGHT,
    MAX_US_ID
};

/* Function prototypes */
//void ping_ultrasonic_sensors(int timeout);
void ultrasonic_init(void);
void ping_ultrasonic_sensors(int timeout, sensor_response *response);



#endif /* L5_APPLICATION_ULTRASONIC_HPP_ */
