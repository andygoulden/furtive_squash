# Remove built-in rules and variables (which can cause annoying bugs), and warn on undefined variables (again to avoid bugs)
MAKEFLAGS += --no-builtin-rules --no-builtin-variables --warn-undefined-variables

# Parallelise, for faster builds (but sometimes annoying if build failing)
MAKEFLAGS+=-j

.DEFAULT_GOAL:=stm32f7_hal

TARGET:=stm32f7_hal.a
PLATFORM:=stm32f756_nucleo

PROJECT_ROOT:=$(realpath ../)

include $(PROJECT_ROOT)/scripts/include.mk

C_FLAGS+= -DUSE_FULL_LL_DRIVER -DSTM32F756xx
C_FLAGS+= -DUSE_HAL_DRIVER

THIRD_PARTY_PATH:=$(PROJECT_ROOT)/third_party
STM32_HAL_REPO_PATH:=$(THIRD_PARTY_PATH)/stm32f7xx_hal_driver
CMSIS_REPO_PATH:=$(THIRD_PARTY_PATH)/CMSIS_5
CMSIS_F7_PATH:=$(THIRD_PARTY_PATH)/cmsis-device-f7

BUILD_DIR_RELEASE:=$(THIRD_PARTY_PATH)/build_stm32f7_hal_release
BUILD_DIR_DEBUG:=$(THIRD_PARTY_PATH)/build_stm32f7_hal_debug

MAKEFILE:=$(THIRD_PARTY_PATH)/stm32f7_hal.mk

ifeq ($(DEBUG),1)
OPT = -Og
BUILD_DIR:=$(BUILD_DIR_DEBUG)
else
OPT = -O2
BUILD_DIR:=$(BUILD_DIR_RELEASE)
endif

STM32_HAL_C_SOURCES_ALL:=$(wildcard $(STM32_HAL_REPO_PATH)/Src/*.c)
STM32_HAL_TEMPLATES:=$(wildcard $(STM32_HAL_REPO_PATH)/Src/*_template.c)
STM32_HAL_C_SOURCES:=$(filter-out $(STM32_HAL_TEMPLATES),$(STM32_HAL_C_SOURCES_ALL))
# STM32_HAL_C_SOURCES:=$(wildcard $(STM32_HAL_REPO_PATH)/Src/stm32f7xx_ll_*.c)

STM32_HAL_OBJECTS:=$(addprefix $(BUILD_DIR)/,$(notdir $(STM32_HAL_C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(STM32_HAL_C_SOURCES)))

STM32_HAL_INCLUDES:=\
-I$(STM32_HAL_REPO_PATH)/Inc \
-I$(PROJECT_ROOT)/src/platform/$(PLATFORM) \
-I$(CMSIS_REPO_PATH)/CMSIS/Core/Include \
-I $(CMSIS_F7_PATH)/Include

.PHONY: stm32f7_hal
stm32f7_hal: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(STM32_HAL_OBJECTS) $(MAKEFILE) | $(BUILD_DIR)
	@echo "AR $@"
	@$(AR) -crs $@ $(STM32_HAL_OBJECTS)

$(BUILD_DIR)/%.o: %.c $(MAKEFILE) | $(BUILD_DIR)
	@echo "CC $@"
	@$(CC) -c $(C_FLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $(STM32_HAL_INCLUDES) $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	-rm -fR $(BUILD_DIR_DEBUG) $(BUILD_DIR_RELEASE)
