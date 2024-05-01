################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/userFiles/inits/inits.c 

OBJS += \
./Core/Src/userFiles/inits/inits.o 

C_DEPS += \
./Core/Src/userFiles/inits/inits.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/userFiles/inits/%.o Core/Src/userFiles/inits/%.su Core/Src/userFiles/inits/%.cyclo: ../Core/Src/userFiles/inits/%.c Core/Src/userFiles/inits/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F105xC -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Maizel/STM32CubeIDE/workspace_1.11.0/RightCANChipTestBigFilter/Core/Src/userFiles/can" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-userFiles-2f-inits

clean-Core-2f-Src-2f-userFiles-2f-inits:
	-$(RM) ./Core/Src/userFiles/inits/inits.cyclo ./Core/Src/userFiles/inits/inits.d ./Core/Src/userFiles/inits/inits.o ./Core/Src/userFiles/inits/inits.su

.PHONY: clean-Core-2f-Src-2f-userFiles-2f-inits

