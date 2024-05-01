################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/connectionCheck/can/canConfig.c \
../Core/Src/connectionCheck/can/canRx.c \
../Core/Src/connectionCheck/can/canTx.c 

OBJS += \
./Core/Src/connectionCheck/can/canConfig.o \
./Core/Src/connectionCheck/can/canRx.o \
./Core/Src/connectionCheck/can/canTx.o 

C_DEPS += \
./Core/Src/connectionCheck/can/canConfig.d \
./Core/Src/connectionCheck/can/canRx.d \
./Core/Src/connectionCheck/can/canTx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/connectionCheck/can/%.o Core/Src/connectionCheck/can/%.su: ../Core/Src/connectionCheck/can/%.c Core/Src/connectionCheck/can/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F105xC -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-connectionCheck-2f-can

clean-Core-2f-Src-2f-connectionCheck-2f-can:
	-$(RM) ./Core/Src/connectionCheck/can/canConfig.d ./Core/Src/connectionCheck/can/canConfig.o ./Core/Src/connectionCheck/can/canConfig.su ./Core/Src/connectionCheck/can/canRx.d ./Core/Src/connectionCheck/can/canRx.o ./Core/Src/connectionCheck/can/canRx.su ./Core/Src/connectionCheck/can/canTx.d ./Core/Src/connectionCheck/can/canTx.o ./Core/Src/connectionCheck/can/canTx.su

.PHONY: clean-Core-2f-Src-2f-connectionCheck-2f-can

