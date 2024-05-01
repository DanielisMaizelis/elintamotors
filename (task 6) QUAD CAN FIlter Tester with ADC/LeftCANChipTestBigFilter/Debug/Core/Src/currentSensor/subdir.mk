################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/currentSensor/currentSensor.c 

OBJS += \
./Core/Src/currentSensor/currentSensor.o 

C_DEPS += \
./Core/Src/currentSensor/currentSensor.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/currentSensor/%.o Core/Src/currentSensor/%.su: ../Core/Src/currentSensor/%.c Core/Src/currentSensor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F105xC -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-currentSensor

clean-Core-2f-Src-2f-currentSensor:
	-$(RM) ./Core/Src/currentSensor/currentSensor.d ./Core/Src/currentSensor/currentSensor.o ./Core/Src/currentSensor/currentSensor.su

.PHONY: clean-Core-2f-Src-2f-currentSensor

