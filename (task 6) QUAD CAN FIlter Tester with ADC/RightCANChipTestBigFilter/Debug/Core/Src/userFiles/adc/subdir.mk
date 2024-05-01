################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/userFiles/adc/adc.c 

OBJS += \
./Core/Src/userFiles/adc/adc.o 

C_DEPS += \
./Core/Src/userFiles/adc/adc.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/userFiles/adc/%.o Core/Src/userFiles/adc/%.su Core/Src/userFiles/adc/%.cyclo: ../Core/Src/userFiles/adc/%.c Core/Src/userFiles/adc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F105xC -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-userFiles-2f-adc

clean-Core-2f-Src-2f-userFiles-2f-adc:
	-$(RM) ./Core/Src/userFiles/adc/adc.cyclo ./Core/Src/userFiles/adc/adc.d ./Core/Src/userFiles/adc/adc.o ./Core/Src/userFiles/adc/adc.su

.PHONY: clean-Core-2f-Src-2f-userFiles-2f-adc

