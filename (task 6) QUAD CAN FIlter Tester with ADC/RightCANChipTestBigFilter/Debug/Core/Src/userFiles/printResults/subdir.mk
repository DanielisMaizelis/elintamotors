################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/userFiles/printResults/print.c 

OBJS += \
./Core/Src/userFiles/printResults/print.o 

C_DEPS += \
./Core/Src/userFiles/printResults/print.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/userFiles/printResults/%.o Core/Src/userFiles/printResults/%.su Core/Src/userFiles/printResults/%.cyclo: ../Core/Src/userFiles/printResults/%.c Core/Src/userFiles/printResults/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F105xC -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-userFiles-2f-printResults

clean-Core-2f-Src-2f-userFiles-2f-printResults:
	-$(RM) ./Core/Src/userFiles/printResults/print.cyclo ./Core/Src/userFiles/printResults/print.d ./Core/Src/userFiles/printResults/print.o ./Core/Src/userFiles/printResults/print.su

.PHONY: clean-Core-2f-Src-2f-userFiles-2f-printResults

