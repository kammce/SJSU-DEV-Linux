################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
../L5_Application/source/handlers.cpp \
../L5_Application/source/high_level_init.cpp \
../L5_Application/source/remote.cpp \
../L5_Application/source/terminal.cpp \
../L5_Application/source/wireless.cpp

OBJS += \
./L5_Application/source/handlers.o \
./L5_Application/source/high_level_init.o \
./L5_Application/source/remote.o \
./L5_Application/source/terminal.o \
./L5_Application/source/wireless.o

CPP_DEPS += \
./L5_Application/source/handlers.d \
./L5_Application/source/high_level_init.d \
./L5_Application/source/remote.d \
./L5_Application/source/terminal.d \
./L5_Application/source/wireless.d


# Each subdirectory must supply rules for building sources it contributes
L5_Application/source/%.o: ../L5_Application/source/%.cpp
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