################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../L1_FreeRTOS/src/croutine.c \
../L1_FreeRTOS/src/list.c \
../L1_FreeRTOS/src/queue.c \
../L1_FreeRTOS/src/tasks.c \
../L1_FreeRTOS/src/timers.c

OBJS += \
./L1_FreeRTOS/src/croutine.o \
./L1_FreeRTOS/src/list.o \
./L1_FreeRTOS/src/queue.o \
./L1_FreeRTOS/src/tasks.o \
./L1_FreeRTOS/src/timers.o

C_DEPS += \
./L1_FreeRTOS/src/croutine.d \
./L1_FreeRTOS/src/list.d \
./L1_FreeRTOS/src/queue.d \
./L1_FreeRTOS/src/tasks.d \
./L1_FreeRTOS/src/timers.d


# Each subdirectory must supply rules for building sources it contributes
L1_FreeRTOS/src/%.o: ../L1_FreeRTOS/src/%.c
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


