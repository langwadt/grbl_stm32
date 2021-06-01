######################################
# target
######################################
TARGET = grbl_stm32


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
src/system_stm32f4xx.c \
src/syscalls.c \
src/util/delay.c \
src/util/floatunsisf.c \
src/avr/eeprom.c \
src/avr/io.c \
src/grbl/coolant_control.c \
src/grbl/gcode.c \
src/grbl/limits.c \
src/grbl/main.c \
src/grbl/motion_control.c \
src/grbl/nuts_bolts.c \
src/grbl/planner.c \
src/grbl/print.c \
src/grbl/probe.c \
src/grbl/protocol.c \
src/grbl/report.c \
src/grbl/serial.c \
src/grbl/settings.c \
src/grbl/spindle_control.c \
src/grbl/stepper.c \
src/grbl/system.c \
StdPeriph_Driver/src/misc.c \
StdPeriph_Driver/src/stm32f4xx_adc.c \
StdPeriph_Driver/src/stm32f4xx_can.c \
StdPeriph_Driver/src/stm32f4xx_crc.c \
StdPeriph_Driver/src/stm32f4xx_cryp.c \
StdPeriph_Driver/src/stm32f4xx_cryp_aes.c \
StdPeriph_Driver/src/stm32f4xx_cryp_des.c \
StdPeriph_Driver/src/stm32f4xx_cryp_tdes.c \
StdPeriph_Driver/src/stm32f4xx_dac.c \
StdPeriph_Driver/src/stm32f4xx_dbgmcu.c \
StdPeriph_Driver/src/stm32f4xx_dcmi.c \
StdPeriph_Driver/src/stm32f4xx_dma.c \
StdPeriph_Driver/src/stm32f4xx_dma2d.c \
StdPeriph_Driver/src/stm32f4xx_exti.c \
StdPeriph_Driver/src/stm32f4xx_flash.c \
StdPeriph_Driver/src/stm32f4xx_flash_ramfunc.c \
StdPeriph_Driver/src/stm32f4xx_gpio.c \
StdPeriph_Driver/src/stm32f4xx_hash.c \
StdPeriph_Driver/src/stm32f4xx_hash_md5.c \
StdPeriph_Driver/src/stm32f4xx_hash_sha1.c \
StdPeriph_Driver/src/stm32f4xx_i2c.c \
StdPeriph_Driver/src/stm32f4xx_iwdg.c \
StdPeriph_Driver/src/stm32f4xx_ltdc.c \
StdPeriph_Driver/src/stm32f4xx_pwr.c \
StdPeriph_Driver/src/stm32f4xx_rcc.c \
StdPeriph_Driver/src/stm32f4xx_rng.c \
StdPeriph_Driver/src/stm32f4xx_rtc.c \
StdPeriph_Driver/src/stm32f4xx_sai.c \
StdPeriph_Driver/src/stm32f4xx_sdio.c \
StdPeriph_Driver/src/stm32f4xx_spi.c \
StdPeriph_Driver/src/stm32f4xx_syscfg.c \
StdPeriph_Driver/src/stm32f4xx_tim.c \
StdPeriph_Driver/src/stm32f4xx_usart.c \
StdPeriph_Driver/src/stm32f4xx_wwdg.c 

# ASM sources
ASM_SOURCES =  \
startup/startup_stm32f411xe.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

FLASHER = st-flash
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DSTM32F4 \
-DSTM32 \
-DNUCLEO_F411RE \
-DSTM32F411RETx \
-DDEBUG \
-DUSE_STDPERIPH_DRIVER \
-DSTM32F411xE \
-DUSE_HSE_BYPASS \
-DHSE_VALUE=8000000 \


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-Iinc \
-Isrc/util \
-Isrc/grbl \
-Isrc/avr \
-Isrc \
-IStdPeriph_Driver/inc \
-ICMSIS/core \
-ICMSIS/device


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = LinkerScript.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# flash clears settings !!!!!
#######################################
flash: all
	@echo "don't use st-flash unless you want to erase emulated EEPROM too!!!"
#$(FLASHER) write $(BUILD_DIR)/$(TARGET).bin  0x8000000

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
