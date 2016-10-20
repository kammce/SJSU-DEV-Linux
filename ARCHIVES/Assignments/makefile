CC 				= arm-none-eabi-gcc
CPPC 			= arm-none-eabi-g++

OBJ_DIR			= obj
SRC_DIR			= src
BIN_DIR			= bin

CFLAGS = -mcpu=cortex-m3 \
    -mthumb -Os -fmessage-length=0 \
    -ffunction-sections -fdata-sections \
    -Wall -Wshadow -Wlogical-op \
    -Wfloat-equal -DBUILD_CFG_MPU=0 \
    -fabi-version=0 \
    -fno-exceptions \
    -I"$(SRC_DIR)/" \
    -I"$(SRC_DIR)/newlib" \
    -I"$(SRC_DIR)/L0_LowLevel" \
    -I"$(SRC_DIR)/L1_FreeRTOS" \
    -I"$(SRC_DIR)/L1_FreeRTOS/include" \
    -I"$(SRC_DIR)/L1_FreeRTOS/portable" \
    -I"$(SRC_DIR)/L1_FreeRTOS/portable/no_mpu" \
    -I"$(SRC_DIR)/L2_Drivers" \
    -I"$(SRC_DIR)/L2_Drivers/base" \
    -I"$(SRC_DIR)/L3_Utils" \
    -I"$(SRC_DIR)/L3_Utils/tlm" \
    -I"$(SRC_DIR)/L4_IO" \
    -I"$(SRC_DIR)/L4_IO/fat" \
    -I"$(SRC_DIR)/L4_IO/wireless" \
    -I"$(SRC_DIR)/L5_Application"

SOURCE 			= $(shell find $(SRC_DIR) -name '*.c' -o -name '*.cpp')
# $(patsubst %.cpp,%.o, LIST) 		: Replace .cpp -> .o
# $(patsubst %.c,%.o, LIST)			: Replace .c -> .o
# $(patsubst src/%,%, LIST) 		: Replace src/path/file.o -> path/file.o
# $(addprefix $(OBJ_DIR)/, LIST) 	: Add OBJ DIR to path (path/file.o -> obj/path/file.o)
OBJECT_FILES 	= $(addprefix $(OBJ_DIR)/, $(patsubst $(SRC_DIR)/%,%, $(patsubst %.c,%.o, $(patsubst %.cpp,%.o, $(SOURCE)))))
EXECUTABLE		= $(BIN_DIR)/lpc1758_freertos.elf
HEX				= $(EXECUTABLE:.elf=.hex)
LIST			= $(EXECUTABLE:.elf=.lst)
SIZE			= $(EXECUTABLE:.elf=.siz)
MAP				= $(EXECUTABLE:.elf=.map)

all: $(OBJ_DIR) $(BIN_DIR) $(HEX)

.PHONY: all clean
# python ../_can_dbc/dbc_parse.py -i ../_can_dbc/243.dbc -s DRIVER > ../_can_dbc/generated_can.h

$(HEX): $(SIZE)
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "$(EXECUTABLE)" "$(HEX)"
	@echo 'Finished building: $@'
	@echo ' '

$(SIZE): $(LIST)
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "$(EXECUTABLE)"
	@echo 'Finished building: $@'
	@echo ' '

$(LIST): $(EXECUTABLE)
	@echo 'Invoking: Cross ARM GNU Create Listing'
	arm-none-eabi-objdump --source --all-headers --demangle --line-numbers --wide "$(EXECUTABLE)" > "$(LIST)"
	@echo 'Finished building: $@'
	@echo ' '

$(EXECUTABLE): $(OBJECT_FILES)
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wshadow -Wlogical-op -Wfloat-equal -T loader.ld -nostartfiles -Xlinker --gc-sections -Wl,-Map,"$(MAP)" -specs=nano.specs -o "$(EXECUTABLE)" $(OBJECT_FILES)
	@echo 'Finished building target: $@'
	@echo ' '

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	mkdir -p "$(dir $@)"
	$(CPPC) $(CFLAGS) -std=gnu++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJ_DIR):
	@echo 'Creating Objects Folder: $<'
	mkdir $(OBJ_DIR)

$(BIN_DIR):
	@echo 'Creating Binary Folder: $<'
	mkdir $(BIN_DIR)

clean:
	rm -R obj bin