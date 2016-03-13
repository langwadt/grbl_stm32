################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f411xe.s 

OBJS += \
./startup/startup_stm32f411xe.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo %cd%
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"D:/git/grbl_stm32/inc" -I"D:/git/grbl_stm32/CMSIS/core" -I"D:/git/grbl_stm32/CMSIS/device" -I"D:/git/grbl_stm32/StdPeriph_Driver/inc" -I"D:/git/grbl_stm32/src" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


