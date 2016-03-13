################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/avr/eeprom.c \
../src/avr/io.c 

OBJS += \
./src/avr/eeprom.o \
./src/avr/io.o 

C_DEPS += \
./src/avr/eeprom.d \
./src/avr/io.d 


# Each subdirectory must supply rules for building sources it contributes
src/avr/%.o: ../src/avr/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F4 -DSTM32 -DNUCLEO_F411RE -DSTM32F411RETx -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F411xE -I"D:/git/grbl_stm32/inc" -I"D:/git/grbl_stm32/CMSIS/core" -I"D:/git/grbl_stm32/CMSIS/device" -I"D:/git/grbl_stm32/StdPeriph_Driver/inc" -I"D:/git/grbl_stm32/src" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


