CC = arm-none-eabi-gcc
CPPC = arm-none-eabi-g++
CFLAGS = -mcpu=cortex-m3 \
    -mthumb -Os -fmessage-length=0 \
    -ffunction-sections -fdata-sections \
    -Wall -Wshadow -Wlogical-op \
    -Wfloat-equal -DBUILD_CFG_MPU=0 \
    -fabi-version=0 \
    -fno-exceptions \
    -I"./" \
    -I"./newlib" \
    -I"./L0_LowLevel" \
    -I"./L1_FreeRTOS" \
    -I"./L1_FreeRTOS/include" \
    -I"./L1_FreeRTOS/portable" \
    -I"./L2_Drivers" \
    -I"./L2_Drivers/base" \
    -I"./L3_Utils" \
    -I"./L3_Utils/tlm" \
    -I"./L4_IO" \
    -I"./L4_IO/fat" \
    -I"./L4_IO/wireless" \
    -I"./L5_Application"

SOURCE=$(shell find . -name '*.c' -o -name '*.cpp')
OBJECTS=$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))
D_FILES=$(OBJECTS:.o=.d)

EXECUTABLE=lpc1758_freertos.elf
HEX=$(EXECUTABLE:.elf=.hex)
LIST=$(EXECUTABLE:.elf=.lst)
SIZE=$(EXECUTABLE:.elf=.siz)
MAP=$(EXECUTABLE:.elf=.map)

all: $(SOURCE) $(EXECUTABLE) $(HEX) $(LIST) $(SIZE)

lpc1758_freertos.elf: $(OBJECTS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wshadow -Wlogical-op -Wfloat-equal -T loader.ld -nostartfiles -Xlinker --gc-sections -Wl,-Map,"$(MAP)" -specs=nano.specs -o "lpc1758_freertos.elf" $(OBJECTS)
	@echo 'Finished building target: $@'
	@echo ' '

lpc1758_freertos.hex: lpc1758_freertos.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "lpc1758_freertos.elf"  "lpc1758_freertos.hex"
	@echo 'Finished building: $@'
	@echo ' '

lpc1758_freertos.lst: lpc1758_freertos.elf
	@echo 'Invoking: Cross ARM GNU Create Listing'
	arm-none-eabi-objdump --source --all-headers --demangle --line-numbers --wide "lpc1758_freertos.elf" > "lpc1758_freertos.lst"
	@echo 'Finished building: $@'
	@echo ' '

lpc1758_freertos.siz: lpc1758_freertos.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "lpc1758_freertos.elf"
	@echo 'Finished building: $@'
	@echo ' '

.cpp.o:
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	$(CPPC) $(CFLAGS) -std=gnu++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

.c.o:
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	$(CC) $(CFLAGS) -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

clean:
	-rm -f $(OBJECTS) $(D_FILES) $(EXECUTABLE) $(HEX) $(LIST) $(SIZE) $(MAP)