################################################################################
# Automatically-generated file. But you can edit!!!
################################################################################

-include ../makefile.init

RM := rm -rf

# Output name

PROJECT = Executables/sjsud_exec

# All of the sources participating in the build are defined here
-include sources.mk
-include newlib/subdir.mk
-include L5_Application/source/subdir.mk
-include L5_Application/examples/subdir.mk
-include L5_Application/subdir.mk
-include L4_IO/wireless/src/subdir.mk
-include L4_IO/src/subdir.mk
-include L4_IO/fat/option/subdir.mk
-include L4_IO/fat/disk/subdir.mk
-include L4_IO/fat/subdir.mk
-include L3_Utils/tlm/src/subdir.mk
-include L3_Utils/src/subdir.mk
-include L2_Drivers/src/subdir.mk
-include L1_FreeRTOS/src/subdir.mk
-include L1_FreeRTOS/portable/subdir.mk
-include L1_FreeRTOS/hooks/subdir.mk
-include L1_FreeRTOS/MemMang/subdir.mk
-include L0_LowLevel/source/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables
SECONDARY_FLASH += \
$(PROJECT).hex \

SECONDARY_LIST += \
$(PROJECT).lst \

SECONDARY_SIZE += \
$(PROJECT).siz \


# All Target
all: $(PROJECT).elf secondary-outputs
	

# Tool invocations
$(PROJECT).elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM GCC C++ Linker'
	arm-none-eabi-g++ -T"../loader.ld" -nostartfiles -Wl,-Map,$(PROJECT).map -mcpu=cortex-m3 -mthumb -o "$(PROJECT).elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(PROJECT).hex: $(PROJECT).elf
	@echo 'Invoking: ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex $(PROJECT).elf  "$(PROJECT).hex"
	@echo 'Finished building: $@'
	@echo ' '

$(PROJECT).lst: $(PROJECT).elf
	@echo 'Invoking: ARM GNU Create Listing'
	arm-none-eabi-objdump -h -S $(PROJECT).elf > "$(PROJECT).lst"
	@echo 'Finished building: $@'
	@echo ' '

$(PROJECT).siz: $(PROJECT).elf
	@echo 'Invoking: ARM GNU Print Size'
	arm-none-eabi-size  --format=berkeley $(PROJECT).elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(SECONDARY_FLASH)$(CXX_DEPS)$(S_UPPER_DEPS)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(C++_DEPS)$(ASM_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS) $(PROJECT).elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

move-executables:
	@echo 'Moving: Binaries to Executable folder'
	mv $(PROJECT).elf $(PROJECT).hex $(PROJECT).lst $(PROJECT).map Executables

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
