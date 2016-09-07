################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
../L2_Drivers/src/i2c2.cpp \
../L2_Drivers/src/i2c_base.cpp \
../L2_Drivers/src/lpc_pwm.cpp \
../L2_Drivers/src/uart0.cpp \
../L2_Drivers/src/uart2.cpp \
../L2_Drivers/src/uart3.cpp \
../L2_Drivers/src/uart_dev.cpp \
../L2_Drivers/base/char_dev.hpp

C_SRCS += \
../L2_Drivers/src/can.c \
../L2_Drivers/src/eint.c \
../L2_Drivers/src/rtc.c \
../L2_Drivers/src/spi1.c \
../L2_Drivers/src/spi_dma.c

OBJS += \
./L2_Drivers/src/can.o \
./L2_Drivers/src/eint.o \
./L2_Drivers/src/i2c2.o \
./L2_Drivers/src/i2c_base.o \
./L2_Drivers/src/lpc_pwm.o \
./L2_Drivers/src/rtc.o \
./L2_Drivers/src/spi1.o \
./L2_Drivers/src/spi_dma.o \
./L2_Drivers/src/uart0.o \
./L2_Drivers/src/uart2.o \
./L2_Drivers/src/uart3.o \
./L2_Drivers/src/uart_dev.o

C_DEPS += \
./L2_Drivers/src/can.d \
./L2_Drivers/src/eint.d \
./L2_Drivers/src/rtc.d \
./L2_Drivers/src/spi1.d \
./L2_Drivers/src/spi_dma.d

CPP_DEPS += \
./L2_Drivers/src/i2c2.d \
./L2_Drivers/src/i2c_base.d \
./L2_Drivers/src/lpc_pwm.d \
./L2_Drivers/src/uart0.d \
./L2_Drivers/src/uart2.d \
./L2_Drivers/src/uart3.d \
./L2_Drivers/src/uart_dev.d


# Each subdirectory must supply rules for building sources it contributes
L2_Drivers/src/%.o: ../L2_Drivers/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM GCC C Compiler'
	arm-none-eabi-gcc \
	-I"../" \
	-I"../newlib" \
	-I"../L0_LowLevel" \
	-I"../L1_FreeRTOS" \
	-I"../L1_FreeRTOS/include" \
	-I"../L1_FreeRTOS/portable" \
	-I"../L2_Drivers" \
	-I"../L3_Utils" \
	-I"../L3_Utils/tlm" \
	-I"../L4_IO" \
	-I"../L4_IO/fat" \
	-I"../L4_IO/wireless" \
	-I"../L5_Application" \
	-Os -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -fno-strict-aliasing -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

L2_Drivers/src/%.o: ../L2_Drivers/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM GCC C++ Compiler'
	arm-none-eabi-g++ \
	-I"../" \
	-I"../newlib" \
	-I"../L0_LowLevel" \
	-I"../L1_FreeRTOS" \
	-I"../L1_FreeRTOS/include" \
	-I"../L1_FreeRTOS/portable" \
	-I"../L2_Drivers" \
	-I"../L3_Utils" \
	-I"../L3_Utils/tlm" \
	-I"../L4_IO" \
	-I"../L4_IO/fat" \
	-I"../L4_IO/wireless" \
	-I"../L5_Application" \
	-Os -Wall -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


