################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/connectionCheck/lin/linRx.c \
../Core/Src/connectionCheck/lin/linTx.c 

OBJS += \
./Core/Src/connectionCheck/lin/linRx.o \
./Core/Src/connectionCheck/lin/linTx.o 

C_DEPS += \
./Core/Src/connectionCheck/lin/linRx.d \
./Core/Src/connectionCheck/lin/linTx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/connectionCheck/lin/%.o Core/Src/connectionCheck/lin/%.su: ../Core/Src/connectionCheck/lin/%.c Core/Src/connectionCheck/lin/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F105xC -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-connectionCheck-2f-lin

clean-Core-2f-Src-2f-connectionCheck-2f-lin:
	-$(RM) ./Core/Src/connectionCheck/lin/linRx.d ./Core/Src/connectionCheck/lin/linRx.o ./Core/Src/connectionCheck/lin/linRx.su ./Core/Src/connectionCheck/lin/linTx.d ./Core/Src/connectionCheck/lin/linTx.o ./Core/Src/connectionCheck/lin/linTx.su

.PHONY: clean-Core-2f-Src-2f-connectionCheck-2f-lin

