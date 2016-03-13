################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/grbl/coolant_control.c \
../src/grbl/gcode.c \
../src/grbl/limits.c \
../src/grbl/main.c \
../src/grbl/motion_control.c \
../src/grbl/nuts_bolts.c \
../src/grbl/planner.c \
../src/grbl/print.c \
../src/grbl/probe.c \
../src/grbl/protocol.c \
../src/grbl/report.c \
../src/grbl/serial.c \
../src/grbl/settings.c \
../src/grbl/spindle_control.c \
../src/grbl/stepper.c \
../src/grbl/system.c 

OBJS += \
./src/grbl/coolant_control.o \
./src/grbl/gcode.o \
./src/grbl/limits.o \
./src/grbl/main.o \
./src/grbl/motion_control.o \
./src/grbl/nuts_bolts.o \
./src/grbl/planner.o \
./src/grbl/print.o \
./src/grbl/probe.o \
./src/grbl/protocol.o \
./src/grbl/report.o \
./src/grbl/serial.o \
./src/grbl/settings.o \
./src/grbl/spindle_control.o \
./src/grbl/stepper.o \
./src/grbl/system.o 

C_DEPS += \
./src/grbl/coolant_control.d \
./src/grbl/gcode.d \
./src/grbl/limits.d \
./src/grbl/main.d \
./src/grbl/motion_control.d \
./src/grbl/nuts_bolts.d \
./src/grbl/planner.d \
./src/grbl/print.d \
./src/grbl/probe.d \
./src/grbl/protocol.d \
./src/grbl/report.d \
./src/grbl/serial.d \
./src/grbl/settings.d \
./src/grbl/spindle_control.d \
./src/grbl/stepper.d \
./src/grbl/system.d 


# Each subdirectory must supply rules for building sources it contributes
src/grbl/%.o: ../src/grbl/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F4 -DSTM32 -DNUCLEO_F411RE -DSTM32F411RETx -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F411xE -I"D:/git/grbl_stm32/inc" -I"D:/git/grbl_stm32/CMSIS/core" -I"D:/git/grbl_stm32/CMSIS/device" -I"D:/git/grbl_stm32/StdPeriph_Driver/inc" -I"D:/git/grbl_stm32/src" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


