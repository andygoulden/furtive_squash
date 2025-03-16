# Remove built-in rules and variables (which can cause annoying bugs), and warn on undefined variables (again to avoid bugs)
MAKEFLAGS += --no-builtin-rules --no-builtin-variables --warn-undefined-variables

# Parallelise, for faster builds (but sometimes annoying if build failing)
MAKEFLAGS+=-j

.DEFAULT_GOAL:=sx126x

TARGET:=sx126x.a
PLATFORM:=stm32f756_nucleo

PROJECT_ROOT:=$(realpath ../)

include $(PROJECT_ROOT)/scripts/include.mk

C_FLAGS+= -DSTM32F756xx

THIRD_PARTY_PATH:=$(PROJECT_ROOT)/third_party
SX126X_REPO_PATH:=$(THIRD_PARTY_PATH)/sx126x_driver

BUILD_DIR_RELEASE:=$(THIRD_PARTY_PATH)/build_sx126x_release
BUILD_DIR_DEBUG:=$(THIRD_PARTY_PATH)/build_sx126x_debug

MAKEFILE:=$(THIRD_PARTY_PATH)/sx126x.mk

ifeq ($(DEBUG),1)
OPT = -Og
BUILD_DIR:=$(BUILD_DIR_DEBUG)
else
OPT = -O2
BUILD_DIR:=$(BUILD_DIR_RELEASE)
endif

SX126X_C_SOURCES:=$(wildcard $(SX126X_REPO_PATH)/src/*.c)

SX126X_OBJECTS:=$(addprefix $(BUILD_DIR)/,$(notdir $(SX126X_C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(SX126X_C_SOURCES)))

SX126X_INCLUDES:=\
-I$(SX126X_REPO_PATH)/Inc

.PHONY: sx126x
sx126x: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(SX126X_OBJECTS) $(MAKEFILE) | $(BUILD_DIR)
	@echo "AR $@"
	@$(AR) -crs $@ $(SX126X_OBJECTS)

$(BUILD_DIR)/%.o: %.c $(MAKEFILE) | $(BUILD_DIR)
	@echo "CC $@"
	@$(CC) -c $(C_FLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $(SX126X_INCLUDES) $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	-rm -fR $(BUILD_DIR_DEBUG) $(BUILD_DIR_RELEASE)
