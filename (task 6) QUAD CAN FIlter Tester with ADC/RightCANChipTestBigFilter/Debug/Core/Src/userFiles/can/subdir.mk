################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/userFiles/can/canConfig.c \
../Core/Src/userFiles/can/canRx.c \
../Core/Src/userFiles/can/canTx.c 

OBJS += \
./Core/Src/userFiles/can/canConfig.o \
./Core/Src/userFiles/can/canRx.o \
./Core/Src/userFiles/can/canTx.o 

C_DEPS += \
./Core/Src/userFiles/can/canConfig.d \
./Core/Src/userFiles/can/canRx.d \
./Core/Src/userFiles/can/canTx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/userFiles/can/%.o Core/Src/userFiles/can/%.su Core/Src/userFiles/can/%.cyclo: ../Core/Src/userFiles/can/%.c Core/Src/userFiles/can/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F105xC -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-userFiles-2f-can

clean-Core-2f-Src-2f-userFiles-2f-can:
	-$(RM) ./Core/Src/userFiles/can/canConfig.cyclo ./Core/Src/userFiles/can/canConfig.d ./Core/Src/userFiles/can/canConfig.o ./Core/Src/userFiles/can/canConfig.su ./Core/Src/userFiles/can/canRx.cyclo ./Core/Src/userFiles/can/canRx.d ./Core/Src/userFiles/can/canRx.o ./Core/Src/userFiles/can/canRx.su ./Core/Src/userFiles/can/canTx.cyclo ./Core/Src/userFiles/can/canTx.d ./Core/Src/userFiles/can/canTx.o ./Core/Src/userFiles/can/canTx.su

.PHONY: clean-Core-2f-Src-2f-userFiles-2f-can

