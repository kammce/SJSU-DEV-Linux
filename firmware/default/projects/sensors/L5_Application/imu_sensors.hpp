
#include "utilities.h"
#include <stdint.h>
#include "i2c2.hpp"

#define MPU9250_ADDRESS 0x68
#define MAG_ADDRESS 0x0c
#define HMC5883L_READ_ADDR 0x3d
#define HMC5883L_WRITE_ADDR 0x3c
#define    GYRO_FULL_SCALE_250_DPS    0x00
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18

#define    ACC_FULL_SCALE_2_G        0x00
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18

#define IMU IMU_Sensor::getInstance()


struct sensor_data_t {
    int16_t x;
    int16_t y;
    int16_t z;
};

class IMU_Sensor : public SingletonTemplate<IMU_Sensor>
{
    public:
        bool init();
        bool getXYZ();
        sensor_data_t Accel_Data, Gyro_Data, Mag_Data;

    private:
        IMU_Sensor()
        {

        }
        friend class SingletonTemplate<IMU_Sensor>;

        I2C_Base& mI2C = I2C2::getInstance();
};
