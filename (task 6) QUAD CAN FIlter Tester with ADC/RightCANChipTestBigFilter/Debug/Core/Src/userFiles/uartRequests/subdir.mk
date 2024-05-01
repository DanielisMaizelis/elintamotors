################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/userFiles/uartRequests/uartRx.c 

OBJS += \
./Core/Src/userFiles/uartRequests/uartRx.o 

C_DEPS += \
./Core/Src/userFiles/uartRequests/uartRx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/userFiles/uartRequests/%.o Core/Src/userFiles/uartRequests/%.su Core/Src/userFiles/uartRequests/%.cyclo: ../Core/Src/userFiles/uartRequests/%.c Core/Src/userFiles/uartRequests/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F105xC -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-userFiles-2f-uartRequests

clean-Core-2f-Src-2f-userFiles-2f-uartRequests:
	-$(RM) ./Core/Src/userFiles/uartRequests/uartRx.cyclo ./Core/Src/userFiles/uartRequests/uartRx.d ./Core/Src/userFiles/uartRequests/uartRx.o ./Core/Src/userFiles/uartRequests/uartRx.su

.PHONY: clean-Core-2f-Src-2f-userFiles-2f-uartRequests

