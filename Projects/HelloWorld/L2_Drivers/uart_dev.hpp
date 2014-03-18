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
 * @brief Provides UART Base class functionality for UART peripherals
 *
 *  10102013 : Make init() public, and protect from re-init leaking memory through xQueueCreate()
 *  05122013 : Added version history
 */
#ifndef UART_DEV_HPP_
#define UART_DEV_HPP_

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "LPC17xx.h"



/**
 * UART Base class that can be used to write drivers for all UART peripherals.
 * Steps needed to write a UART driver:
 *  - Inherit this class
 *  - Call init() and configure PINSEL to select your UART pins
 *  - When your UART(#) hardware interrupt occurs, call handleInterrupt()
 *
 *  To connect UART Interrupt with your UART, reference this example:
 *  @code
 *  extern "C"
*   {
 *      void UART0_IRQHandler()
 *      {
 *          UART0::getInstance().handleInterrupt();
 *      }
 *   }
 *  @endcode
 *
 *  @warning This class hasn't been tested for UART1 due to different memory map.
 *  @ingroup Drivers
 */
class UartDev
{
    public:
        /**
         * Initializes the UART register including Queues, baudrate and hardware.
         * Parent class should call this method before initializing Pin-Connect-Block
         * @param pclk      The system peripheral clock for this UART
         * @param baudRate  The baud rate to set
         * @param rxQSize   The receive queue size
         * @param txQSize   The transmit queue size
         * @post    Sets 8-bit mode, no parity, no flow control.
         * @warning This will not initialize the PINS, so user needs to do pin
         *          selection because LPC's same UART hardware, such as UART2
         *          is available on multiple pins.
         * @note If the txQSize is too small, functions performing printf will start to block.
         */
        bool init(unsigned int pclk, unsigned int baudRate, int rxQSize=32, int txQSize=32);

        /**
         * @{  This API just provides a means to set a flag if UART is ready or not
         *     This doesn't cause any change to the way UART functions.
         */
        bool isReady(void) { return mReady; }
        void setReady(bool r) { mReady = r; }
        /** @} */

        /// Reset the baud-rate after UART has been initialized
        void setBaudRate(unsigned int baudRate);

        /**
         * @returns a character from the UART input
         * @param   pInputChar  The pointer to input char to store received character
         * @param   timeout Optional parameter which defaults to maximum value that
         *          will allow you to wait forever for a character to be received
         * @returns true if a character was obtained within the given timeout
         */
        bool getChar(char* pInputChar, unsigned int timeout=portMAX_DELAY) const;

        /**
         * Outputs a char given by @param out
         * @param   timeout Optional parameter which defaults to maximum value that
         *          will allow you to wait forever for a character to be sent
         * @returns true if the output char was successfully written to Queue, or
         *          false if the output queue was full within the given timeout
         */
        bool putChar(char out, unsigned int timeout=portMAX_DELAY);


        /**
         * @{ Output a null-terminated string
         * puts() will also output newline chars \r\n at the end of the string
         */
        bool put(const char* pString, unsigned int timeout=0xffffffff);
        void putline(const char* pBuff, unsigned int timeout=0xffffffff);
        /** @} */

        /**
         * Get a string of input up to maxLen
         * @param pBuff The buffer to store data to
         * @param maxLen The maximum chars to get
         * @param timeout The timeout in ticks to wait
         */
        bool gets(char* pBuff, int maxLen, unsigned int timeout=0xffffffff);

        /**
         * Just like printf, except it will print to this output interface
         * @returns the number of characters printed
         */
        int printf(const char *format, ...);

        /// Flushed all pending transmission of the uart queue
        void flush(void);

        /**
         * Just like scanf, except this will perform scanf after receiving a line
         * of input using the gets() method.
         * @warning  scanf() requires a \n (newline) terminated char to work.
         */
        int scanf(const char *format, ...);

        /**
         * Get the size for the printf() memory used by printf() function
         */
        inline uint16_t getPrintfMemSize(void) const { return mPrintfMemSize; }

        /**
         * @{ Get the Rx and Tx queue information
         * Watermarks provide the queue's usage to access the capacity usage
         */
        inline unsigned int getRxQueueSize() const { return uxQueueMessagesWaiting(mRxQueue); }
        inline unsigned int getTxQueueSize() const { return uxQueueMessagesWaiting(mTxQueue); }
        inline unsigned int getRxQueueWatermark() const { return mRxQWatermark; }
        inline unsigned int getTxQueueWatermark() const { return mTxQWatermark; }
        /** @} */

        /**
         * @{ Recent activity api
         * Check to see if this UART received any Rx/Tx activity within the last #X OS ticks.
         * @param ms Optional Parameter: Defaults to 3000 milliseconds
         */
        bool recentlyActive(unsigned int ms=3000) const;
        inline portTickType getLastActivityTime(void) const { return mLastActivityTime; }
        inline void resetActivity(void) { mLastActivityTime = xTaskGetMsCount(); }
        /** @} */

        /**
         * When the UART interrupt occurs, this function should be called to handle
         * future action to take due to the interrupt cause.
         */
        void handleInterrupt();

    protected:
        /**
         * Protected constructor that requires parent class to provide UART's
         * base register address for which to operate this UART driver
         */
        UartDev(unsigned int* pUARTBaseAddr);
        UartDev() {} /** Disallowed constructor */
        ~UartDev();

    private:
        LPC_UART_TypeDef* mpUARTRegBase;///< Pointer to UART's memory map
        xQueueHandle mRxQueue;          ///< Queue for UARTs receive buffer
        xQueueHandle mTxQueue;          ///< Queue for UARTs transmit buffer
        uint32_t mPeripheralClock;      ///< Peripheral clock as given by constructor
        uint16_t mRxQWatermark;         ///< Watermark of Rx Queue
        uint16_t mTxQWatermark;         ///< Watermark of Tx Queue
        portTickType mLastActivityTime; ///< updated each time last rx interrupt occurs

        char *mpPrintfMem;            ///< Heap pointer used by printf()
        uint16_t mPrintfMemSize;      ///< Size of heap used by printf()
        xSemaphoreHandle mPrintfLock; ///< Semaphore to lock printf()
        bool mReady;                  ///< You can mark if Uart is ready or not
};




#endif /* UART_DEV_HPP_ */
