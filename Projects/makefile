ENTITY 			?= DRIVER

CC 				= arm-none-eabi-gcc
CPPC 			= arm-none-eabi-g++

PROJ 			?= HelloWorld
OBJ_DIR			= obj
SRC_DIR			= src
LIB_DIR			= lib
BIN_DIR			= bin

CFLAGS = -mcpu=cortex-m3 \
    -mthumb -Os -fmessage-length=0 \
    -ffunction-sections -fdata-sections \
    -Wall -Wshadow -Wlogical-op \
    -Wfloat-equal -DBUILD_CFG_MPU=0 \
    -fabi-version=0 \
    -fno-exceptions \
    -I"$(LIB_DIR)/" \
    -I"$(LIB_DIR)/newlib" \
    -I"$(LIB_DIR)/L0_LowLevel" \
    -I"$(LIB_DIR)/L1_FreeRTOS" \
    -I"$(LIB_DIR)/L1_FreeRTOS/include" \
    -I"$(LIB_DIR)/L1_FreeRTOS/portable" \
    -I"$(LIB_DIR)/L1_FreeRTOS/portable/no_mpu" \
    -I"$(LIB_DIR)/L2_Drivers" \
    -I"$(LIB_DIR)/L2_Drivers/base" \
    -I"$(LIB_DIR)/L3_Utils" \
    -I"$(LIB_DIR)/L3_Utils/tlm" \
    -I"$(LIB_DIR)/L4_IO" \
    -I"$(LIB_DIR)/L4_IO/fat" \
    -I"$(LIB_DIR)/L4_IO/wireless" \
    -I"$(LIB_DIR)/L5_Application" \
    -I"$(SRC_DIR)/$(PROJ)/L2_Drivers" \
    -I"$(SRC_DIR)/$(PROJ)/L3_Utils" \
    -I"$(SRC_DIR)/$(PROJ)/L4_IO" \
    -I"$(SRC_DIR)/$(PROJ)/L5_Application" \

PREBUILD        	= $(LIB_DIR)/_can_dbc/generated_can.h
LIBRARIES			= $(shell find $(LIB_DIR) -name '*.c' -o -name '*.cpp')
SOURCES				= $(shell find $(SRC_DIR)/$(PROJ) -name '*.c' -o -name '*.cpp')
COMPILABLES 		= $(LIBRARIES) $(SOURCES)

# $(patsubst %.cpp,%.o, LIST) 		: Replace .cpp -> .o
# $(patsubst %.c,%.o, LIST)			: Replace .c -> .o
# $(patsubst src/%,%, LIST) 		: Replace src/path/file.o -> path/file.o
# $(addprefix $(OBJ_DIR)/, LIST) 	: Add OBJ DIR to path (path/file.o -> obj/path/file.o)
OBJECT_FILES 		= $(addprefix $(OBJ_DIR)/, $(patsubst $(SRC_DIR)/%,%, $(patsubst %.c,%.o, $(patsubst %.cpp,%.o, $(COMPILABLES)))))
EXECUTABLE			= $(BIN_DIR)/$(PROJ)/$(PROJ).elf
HEX					= $(EXECUTABLE:.elf=.hex)
LIST				= $(EXECUTABLE:.elf=.lst)
SIZE				= $(EXECUTABLE:.elf=.siz)
MAP					= $(EXECUTABLE:.elf=.map)

.PHONY: all clean

all: $(OBJ_DIR) $(BIN_DIR) $(PREBUILD) $(HEX)

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
	mkdir -p "$(dir $@)"
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wshadow -Wlogical-op -Wfloat-equal -T $(LIB_DIR)/loader.ld -nostartfiles -Xlinker --gc-sections -Wl,-Map,"$(MAP)" -specs=nano.specs -o "$(EXECUTABLE)" $(OBJECT_FILES)
	@echo 'Finished building target: $@'
	@echo ' '

$(OBJ_DIR)/$(PROJ)/%.o: $(SRC_DIR)/$(PROJ)/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	mkdir -p "$(dir $@)"
	$(CPPC) $(CFLAGS) -std=gnu++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJ_DIR)/$(PROJ)/%.o: $(SRC_DIR)/$(PROJ)/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJ_DIR)/$(LIB_DIR)/%.o: $(LIB_DIR)/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	mkdir -p "$(dir $@)"
	$(CPPC) $(CFLAGS) -std=gnu++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(OBJ_DIR)/$(LIB_DIR)/%.o: $(LIB_DIR)/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(PREBUILD):
	python $(LIB_DIR)/_can_dbc/dbc_parse.py -i $(LIB_DIR)/_can_dbc/243.dbc -s $(ENTITY) > $(LIB_DIR)/_can_dbc/generated_can.h

$(OBJ_DIR):
	@echo 'Creating Objects Folder: $<'
	mkdir $(OBJ_DIR)

$(BIN_DIR):
	@echo 'Creating Binary Folder: $<'
	mkdir $(BIN_DIR)

clean:
	rm -fR $(OBJ_DIR)/$(PROJ) $(BIN_DIR)/$(PROJ) $(PREBUILD)

spotless:
	rm -fR $(OBJ_DIR)/* $(BIN_DIR)/*