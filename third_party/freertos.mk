# Remove built-in rules and variables (which can cause annoying bugs), and warn on undefined variables (again to avoid bugs)
MAKEFLAGS += --no-builtin-rules --no-builtin-variables --warn-undefined-variables

# Parallelise, for faster builds (but sometimes annoying if build failing)
MAKEFLAGS+=-j

.DEFAULT_GOAL:=freertos

TARGET:=freertos.a
PLATFORM:=stm32f756_nucleo

PROJECT_ROOT:=$(realpath ../)

include $(PROJECT_ROOT)/scripts/include.mk

PLATFORM_DIR:=$(PROJECT_ROOT)/src/platform/$(PLATFORM)

THIRD_PARTY_PATH:=$(PROJECT_ROOT)/third_party
FREERTOS_REPO_PATH:=$(THIRD_PARTY_PATH)/FreeRTOS-Kernel

BUILD_DIR_RELEASE:=$(THIRD_PARTY_PATH)/build_freertos_release
BUILD_DIR_DEBUG:=$(THIRD_PARTY_PATH)/build_freertos_debug

MAKEFILE:=$(THIRD_PARTY_PATH)/freertos.mk

ifeq ($(DEBUG),1)
OPT = -Og
BUILD_DIR:=$(BUILD_DIR_DEBUG)
else
OPT = -O2
BUILD_DIR:=$(BUILD_DIR_RELEASE)
endif

FREERTOS_C_SOURCES:=$(wildcard $(FREERTOS_REPO_PATH)/*.c)
FREERTOS_C_SOURCES+=$(FREERTOS_REPO_PATH)/portable/GCC/ARM_CM7/r0p1/port.c

FREERTOS_OBJECTS:=$(addprefix $(BUILD_DIR)/,$(notdir $(FREERTOS_C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(FREERTOS_C_SOURCES)))

FREERTOS_INCLUDES:=-I$(FREERTOS_REPO_PATH)/include -I$(FREERTOS_REPO_PATH)/portable/GCC/ARM_CM7/r0p1 -I$(PLATFORM_DIR)

.PHONY: freertos
freertos: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(FREERTOS_OBJECTS) $(MAKEFILE) | $(BUILD_DIR)
	@echo "AR $@"
	@$(AR) -crs $@ $(FREERTOS_OBJECTS)

$(BUILD_DIR)/%.o: %.c $(MAKEFILE) | $(BUILD_DIR)
	@echo "CC $@"
	@$(CC) -c $(C_FLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $(FREERTOS_INCLUDES) $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	-rm -fR $(BUILD_DIR_DEBUG) $(BUILD_DIR_RELEASE)
