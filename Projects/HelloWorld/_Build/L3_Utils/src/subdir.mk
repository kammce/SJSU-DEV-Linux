################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
../L3_Utils/src/command_handler.cpp \
../L3_Utils/src/cpp_task.cpp \
../L3_Utils/src/file_logger.cpp \
../L3_Utils/src/freertos_timer.cpp \
../L3_Utils/src/logger.cpp \
../L3_Utils/src/str.cpp

C_SRCS += \
../L3_Utils/src/c_list.c \
../L3_Utils/src/rtc_alarm.c \
../L3_Utils/src/utilities.c

OBJS += \
./L3_Utils/src/c_list.o \
./L3_Utils/src/command_handler.o \
./L3_Utils/src/cpp_task.o \
./L3_Utils/src/file_logger.o \
./L3_Utils/src/freertos_timer.o \
./L3_Utils/src/logger.o \
./L3_Utils/src/rtc_alarm.o \
./L3_Utils/src/str.o \
./L3_Utils/src/utilities.o

C_DEPS += \
./L3_Utils/src/c_list.d \
./L3_Utils/src/rtc_alarm.d \
./L3_Utils/src/utilities.d

CPP_DEPS += \
./L3_Utils/src/command_handler.d \
./L3_Utils/src/cpp_task.d \
./L3_Utils/src/file_logger.d \
./L3_Utils/src/freertos_timer.d \
./L3_Utils/src/logger.d \
./L3_Utils/src/str.d


# Each subdirectory must supply rules for building sources it contributes
L3_Utils/src/%.o: ../L3_Utils/src/%.c
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

L3_Utils/src/%.o: ../L3_Utils/src/%.cpp
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


