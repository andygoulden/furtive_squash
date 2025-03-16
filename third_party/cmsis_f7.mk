# Remove built-in rules and variables (which can cause annoying bugs), and warn on undefined variables (again to avoid bugs)
MAKEFLAGS += --no-builtin-rules --no-builtin-variables --warn-undefined-variables

# Parallelise, for faster builds (but sometimes annoying if build failing)
MAKEFLAGS+=-j

.DEFAULT_GOAL:=cmsis_f7

TARGET:=cmsis_f7.a
PLATFORM:=stm32f756_nucleo

PROJECT_ROOT:=$(realpath ../)

include $(PROJECT_ROOT)/scripts/include.mk

C_FLAGS+= -DSTM32F756xx

THIRD_PARTY_PATH:=$(PROJECT_ROOT)/third_party
CMSIS_F7_PATH:=$(THIRD_PARTY_PATH)/cmsis-device-f7
CMSIS_PATH:=$(THIRD_PARTY_PATH)/CMSIS_5
STM32_HAL_REPO_PATH:=$(THIRD_PARTY_PATH)/stm32f7xx_hal_driver

BUILD_DIR_RELEASE:=$(THIRD_PARTY_PATH)/build_cmsis_f7_release
BUILD_DIR_DEBUG:=$(THIRD_PARTY_PATH)/build_cmsis_f7_debug

MAKEFILE:=$(THIRD_PARTY_PATH)/cmsis_f7.mk

ifeq ($(DEBUG),1)
OPT = -Og
BUILD_DIR:=$(BUILD_DIR_DEBUG)
else
OPT = -O2
BUILD_DIR:=$(BUILD_DIR_RELEASE)
endif

CMSIS_F7_C_SOURCES:=$(CMSIS_F7_PATH)/Source/Templates/system_stm32f7xx.c
CMSIS_F7_OBJECTS:=$(addprefix $(BUILD_DIR)/,$(notdir $(CMSIS_F7_C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(CMSIS_F7_C_SOURCES)))

CMSIS_F7_ASM_SOURCES:=$(CMSIS_F7_PATH)/Source/Templates/gcc/startup_stm32f756xx.s
CMSIS_F7_OBJECTS+=$(addprefix $(BUILD_DIR)/,$(notdir $(CMSIS_F7_ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(CMSIS_F7_ASM_SOURCES)))

CMSIS_F7_INCLUDES:= \
-I $(CMSIS_F7_PATH)/Include \
-I $(CMSIS_PATH)/CMSIS/Core/Include \
-I $(STM32_HAL_REPO_PATH)/Inc \
-I$(PROJECT_ROOT)/src/platform/$(PLATFORM)

C_FLAGS+=$(CMSIS_F7_INCLUDES)

.PHONY: cmsis_f7
cmsis_f7: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(CMSIS_F7_OBJECTS) $(MAKEFILE) | $(BUILD_DIR)
	@echo "AR $@"
	@$(AR) -crs $@ $(CMSIS_F7_OBJECTS)

$(BUILD_DIR)/%.o: %.c $(MAKEFILE) | $(BUILD_DIR)
	@echo "CC $@"
	@$(CC) -c $(C_FLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $(CMSIS_F7_INCLUDES) $< -o $@

$(BUILD_DIR)/%.o: %.s $(MAKEFILE) | $(BUILD_DIR)
	@echo "AS $@"
	@$(AS) -c $(C_FLAGS) $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	-rm -fR $(BUILD_DIR_DEBUG) $(BUILD_DIR_RELEASE)
