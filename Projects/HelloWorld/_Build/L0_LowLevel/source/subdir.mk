################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
CPP_SRCS += \
../L0_LowLevel/source/cr_startup_lpc175x.cpp \
../L0_LowLevel/source/sys_clock.cpp \
../L0_LowLevel/source/system_init.cpp

C_SRCS += \
../L0_LowLevel/source/core_cm3.c \
../L0_LowLevel/source/lpc_sys.c \
../L0_LowLevel/source/uart0_min.c

OBJS += \
./L0_LowLevel/source/core_cm3.o \
./L0_LowLevel/source/cr_startup_lpc175x.o \
./L0_LowLevel/source/lpc_sys.o \
./L0_LowLevel/source/sys_clock.o \
./L0_LowLevel/source/system_init.o \
./L0_LowLevel/source/uart0_min.o

C_DEPS += \
./L0_LowLevel/source/core_cm3.d \
./L0_LowLevel/source/lpc_sys.d \
./L0_LowLevel/source/uart0_min.d

CPP_DEPS += \
./L0_LowLevel/source/cr_startup_lpc175x.d \
./L0_LowLevel/source/sys_clock.d \
./L0_LowLevel/source/system_init.d


# Each subdirectory must supply rules for building sources it contributes
L0_LowLevel/source/%.o: ../L0_LowLevel/source/%.c
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
	-I"../L5_Application" -Os -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -fno-strict-aliasing -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

L0_LowLevel/source/%.o: ../L0_LowLevel/source/%.cpp
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
	-I"../L5_Application" -Os -Wall -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


