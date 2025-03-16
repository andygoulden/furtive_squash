# Remove built-in rules and variables (which can cause annoying bugs), and warn on undefined variables (again to avoid bugs)
MAKEFLAGS+= --no-builtin-rules --no-builtin-variables --warn-undefined-variables

# Parallelise, for faster builds (but sometimes annoying if build failing)
MAKEFLAGS+=-j

.DEFAULT_GOAL:=all

TARGET:=furtive_squash

PLATFORM:=stm32f756_nucleo

PROJECT_ROOT:=$(realpath ../../)

include $(PROJECT_ROOT)/scripts/include.mk

# Directories
APP_DIR:=$(PROJECT_ROOT)/src/app
LIB_DIR:=$(PROJECT_ROOT)/src/lib
PLATFORM_DIR:=$(PROJECT_ROOT)/src/platform/$(PLATFORM)
LINKERSCRIPT_DIR:=$(PROJECT_ROOT)/src/linkerscript
THIRD_PARTY_PATH:=$(PROJECT_ROOT)/third_party
HAL_DIR:=$(THIRD_PARTY_PATH)/stm32f7xx_hal_driver
CMSIS_DIR:=$(THIRD_PARTY_PATH)/CMSIS_5
FREERTOS_DIR:=$(THIRD_PARTY_PATH)/FreeRTOS-Kernel
CMSIS_F7_DIR:=$(THIRD_PARTY_PATH)/cmsis-device-f7
SX126X_DIR:=$(THIRD_PARTY_PATH)/sx126x_driver

# App build directories
BUILD_DIR_RELEASE:=$(PROJECT_ROOT)/build_release
BUILD_DIR_DEBUG:=$(PROJECT_ROOT)/build_debug

# Third party build directories
HAL_BUILD_DIR_RELEASE:=$(THIRD_PARTY_PATH)/build_stm32f7_hal_release
HAL_BUILD_DIR_DEBUG:=$(THIRD_PARTY_PATH)/build_stm32f7_hal_debug
FREERTOS_BUILD_DIR_RELEASE:=$(THIRD_PARTY_PATH)/build_freertos_release
FREERTOS_BUILD_DIR_DEBUG:=$(THIRD_PARTY_PATH)/build_freertos_debug
CMSIS_F7_BUILD_DIR_RELEASE:=$(THIRD_PARTY_PATH)/build_cmsis_f7_release
CMSIS_F7_BUILD_DIR_DEBUG:=$(THIRD_PARTY_PATH)/build_cmsis_f7_debug
SX126X_BUILD_DIR_RELEASE:=$(THIRD_PARTY_PATH)/build_sx126x_release
SX126X_BUILD_DIR_DEBUG:=$(THIRD_PARTY_PATH)/build_sx126x_debug

ifeq ($(DEBUG),1)
OPT = -Og
BUILD_DIR:=$(BUILD_DIR_DEBUG)
HAL_BUILD_DIR:=$(HAL_BUILD_DIR_DEBUG)
FREERTOS_BUILD_DIR:=$(FREERTOS_BUILD_DIR_DEBUG)
CMSIS_F7_BUILD_DIR:=$(CMSIS_F7_BUILD_DIR_DEBUG)
SX126X_BUILD_DIR:=$(SX126X_BUILD_DIR_DEBUG)
else
OPT = -O2
BUILD_DIR:=$(BUILD_DIR_RELEASE)
HAL_BUILD_DIR:=$(HAL_BUILD_DIR_RELEASE)
FREERTOS_BUILD_DIR:=$(FREERTOS_BUILD_DIR_RELEASE)
CMSIS_F7_BUILD_DIR:=$(CMSIS_F7_BUILD_DIR_RELEASE)
SX126X_BUILD_DIR:=$(SX126X_BUILD_DIR_RELEASE)
endif

MAKEFILE:=$(APP_DIR)/furtive_squash.mk

C_SOURCES:=$(wildcard $(APP_DIR)/*.c)
C_SOURCES+=$(wildcard $(PLATFORM_DIR)/*.c)
C_SOURCES+=$(wildcard $(LIB_DIR)/*/*.c)

C_OBJECTS:=$(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

C_FLAGS += \
-D UART_PRINT=1 \
-I $(APP_DIR) \
-I $(PLATFORM_DIR) \
-I $(HAL_DIR)/Inc \
-I $(CMSIS_DIR)/CMSIS/Core/Include \
-I $(CMSIS_F7_DIR)/Include \
-I $(FREERTOS_DIR)/include \
-I $(FREERTOS_DIR)/portable/GCC/ARM_CM7/r0p1 \
-I $(SX126X_DIR)/src \
-I $(wildcard $(LIB_DIR))

OBJECTS:= $(C_OBJECTS)

C_FLAGS+= -DUSE_FULL_LL_DRIVER -DSTM32F756xx
C_FLAGS+= -DUSE_HAL_DRIVER

# Receive or transmit mode
#
# Options: RX, TX
#
# Default: RX
RX_TX_MODE?=RX
ifeq ($(RX_TX_MODE),RX)
$(info "Rx/Tx mode: Rx")
C_FLAGS+= -DRX_MODE
else ifeq ($(RX_TX_MODE),TX)
$(info "Rx/Tx mode: Tx")
C_FLAGS+= -DTX_MODE
else
$(error "ERROR: invalid RX_TX_MODE")
endif

# Verbose prints on/off (0=off)
VERBOSE?=1
ifeq ($(VERBOSE),1)
C_FLAGS+= -DVERBOSE_PRINTS=1
endif

LD_SCRIPT = $(LINKERSCRIPT_DIR)/STM32F756ZGTx_FLASH.ld

HAL_ARCHIVE:=$(HAL_BUILD_DIR)/stm32f7_hal.a
FREERTOS_ARCHIVE:=$(FREERTOS_BUILD_DIR)/freertos.a
CMSIS_F7_ARCHIVE:=$(CMSIS_F7_BUILD_DIR)/cmsis_f7.a
SX126X_ARCHIVE:=$(SX126X_BUILD_DIR)/sx126x.a

LD_FLAGS:= \
$(MCU) \
-specs=nano.specs \
-T$(LD_SCRIPT) \
-lc \
-lm \
-lnosys \
-Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref \
-Wl,--gc-sections \
$(HAL_ARCHIVE) \
$(FREERTOS_ARCHIVE) \
$(CMSIS_F7_ARCHIVE) \
$(SX126X_ARCHIVE)

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/%.o: %.c $(MAKEFILE) | $(BUILD_DIR)
	@echo "CC $@"
	@$(CC) -c $(C_FLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) $(MAKEFILE)
	@echo "ELF $@"
	@$(CC) $(OBJECTS) $(LD_FLAGS) -o $@
	@$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo "HEX $@"
	@$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo "BIN $@"
	@$(BIN) $< $@

$(BUILD_DIR):
	@echo "DIR $@"
	@mkdir -p $@

.PHONY: flash
flash:
	@openocd -f board/st_nucleo_f7.cfg -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR_DEBUG) $(BUILD_DIR_RELEASE)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)
