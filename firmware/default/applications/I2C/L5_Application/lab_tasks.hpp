#ifndef LAB_TASKS_HPP_
#define LAB_TASKS_HPP_

#include <stdio.h>

#include "scheduler_task.hpp"
#include "i2c2.hpp"
#include "shared_handles.h"

class i2cLab : public scheduler_task
{
public:
    i2cLab(uint8_t priority):
    scheduler_task("i2cLab", 2000, priority)
    {
    	SLAVE_ADDRESS = 0x50;
    	BUFFER_SIZE = 8;
    	buffer = new uint8_t[BUFFER_SIZE];
    	buffer[0] = 0x01;
    	buffer[1] = 0x02;
    	buffer[2] = 0x03;
      buffer[3] = 0x04;
      buffer[4] = 0x05;
      buffer[5] = 0x55;
      buffer[6] = 0x00;
      buffer[7] = 0x00;
    }

    bool init(void)
    {
      mPrintBuffer = xSemaphoreCreateBinary();
      //addSharedObject(shared_printBuffer, mPrintBuffer);
      printf("Initializing slave device to address: %0x\n", SLAVE_ADDRESS);
      I2C2::getInstance().initSlave(SLAVE_ADDRESS, buffer, BUFFER_SIZE);
      return true;
    }

    bool run(void *p)
    {
      // if (xSemaphoreTake(mPrintBuffer, 0))
      // {
        printf("READ DATA:\n");
        for (int i = 0; i < BUFFER_SIZE; ++i)
        {
          printf("[%d]0x%02x::", i, buffer[i]);
        }
        printf("\n");
      // }
      vTaskDelay(2500);
      return true;
    }

private:
  SemaphoreHandle_t mPrintBuffer; ///< Signal that indicates read is complete
  uint8_t SLAVE_ADDRESS;
  int BUFFER_SIZE;
  uint8_t *buffer;
};

#endif /* LAB_TASKS_HPP_ */
