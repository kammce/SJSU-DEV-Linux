################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../L1_FreeRTOS/MemMang/heap_1.c \
../L1_FreeRTOS/MemMang/heap_2.c \
../L1_FreeRTOS/MemMang/heap_3.c \
../L1_FreeRTOS/MemMang/heap_4.c

OBJS += \
./L1_FreeRTOS/MemMang/heap_1.o \
./L1_FreeRTOS/MemMang/heap_2.o \
./L1_FreeRTOS/MemMang/heap_3.o \
./L1_FreeRTOS/MemMang/heap_4.o

C_DEPS += \
./L1_FreeRTOS/MemMang/heap_1.d \
./L1_FreeRTOS/MemMang/heap_2.d \
./L1_FreeRTOS/MemMang/heap_3.d \
./L1_FreeRTOS/MemMang/heap_4.d


# Each subdirectory must supply rules for building sources it contributes
L1_FreeRTOS/MemMang/%.o: ../L1_FreeRTOS/MemMang/%.c
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


