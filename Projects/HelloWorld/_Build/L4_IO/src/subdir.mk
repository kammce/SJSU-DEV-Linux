################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
../L4_IO/src/gpio.cpp \
../L4_IO/src/io_source.cpp \
../L4_IO/src/storage.cpp

OBJS += \
./L4_IO/src/gpio.o \
./L4_IO/src/io_source.o \
./L4_IO/src/storage.o

CPP_DEPS += \
./L4_IO/src/gpio.d \
./L4_IO/src/io_source.d \
./L4_IO/src/storage.d


# Each subdirectory must supply rules for building sources it contributes
L4_IO/src/%.o: ../L4_IO/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM GCC C++ Compiler'
	arm-none-eabi-g++  \
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


