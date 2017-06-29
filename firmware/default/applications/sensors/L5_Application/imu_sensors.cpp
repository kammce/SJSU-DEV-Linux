#include "imu_sensors.hpp"
#include <stdio.h>
#include "utilities.h"
bool IMU_Sensor::init()
{
    mI2C.writeReg(HMC5883L_WRITE_ADDR, 0x00, 0x70); //Configuration regigsters
    mI2C.writeReg(HMC5883L_WRITE_ADDR, 0x01, 0xA0);
    mI2C.writeReg(HMC5883L_WRITE_ADDR, 0x02, 0x00);
    return true; //Add some checking later
}

bool IMU_Sensor::getXYZ()
{
    //Get magnet raw values
    uint8_t buffer[14];
    bool status = true;
    if(mI2C.readRegisters(HMC5883L_READ_ADDR, 0x03, buffer, 6))
    {
        Mag_Data.x = buffer[0] << 8 | buffer[1];
        Mag_Data.z = buffer[2] << 8 | buffer[3];
        Mag_Data.y = buffer[4] << 8 | buffer[5];
    }
    else
    {
        status = false;
    }
    mI2C.writeReg(HMC5883L_WRITE_ADDR, 0x03, 0x00);
    return status;
}

