# Enable debug by default
DEBUG?=1

GCC_PATH?=/opt/gcc-arm-none-eabi-10.3-2021.10/bin
PREFIX = arm-none-eabi-
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
AR = $(GCC_PATH)/$(PREFIX)ar
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
AR = $(PREFIX)ar
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

# STM32 definitions
CPU = -mcpu=cortex-m7
FPU = -mfpu=fpv5-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

AS_FLAGS += $(MCU) $(OPT) -Wall -fdata-sections -ffunction-sections
C_FLAGS += $(CPU) -mthumb $(FPU) $(FLOAT-ABI) $(OPT) -Wall -fdata-sections -ffunction-sections -MMD -MP -MF"$(@:%.o=%.d)"
LD_FLAGS += $(MCU) -specs=nano.specs -Wl,--gc-sections

ifeq ($(DEBUG), 1)
C_FLAGS += -g -gdwarf-2
endif
