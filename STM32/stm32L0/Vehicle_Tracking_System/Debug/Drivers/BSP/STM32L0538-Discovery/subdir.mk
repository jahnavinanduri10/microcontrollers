################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/STM32L0538-Discovery/stm32l0538_discovery.c \
../Drivers/BSP/STM32L0538-Discovery/stm32l0538_discovery_epd.c 

OBJS += \
./Drivers/BSP/STM32L0538-Discovery/stm32l0538_discovery.o \
./Drivers/BSP/STM32L0538-Discovery/stm32l0538_discovery_epd.o 

C_DEPS += \
./Drivers/BSP/STM32L0538-Discovery/stm32l0538_discovery.d \
./Drivers/BSP/STM32L0538-Discovery/stm32l0538_discovery_epd.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32L0538-Discovery/%.o Drivers/BSP/STM32L0538-Discovery/%.su Drivers/BSP/STM32L0538-Discovery/%.cyclo: ../Drivers/BSP/STM32L0538-Discovery/%.c Drivers/BSP/STM32L0538-Discovery/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_STM32L0538_DISCO_REV_B03 -DUSE_HAL_DRIVER -DSTM32L053xx -c -I"D:/stm32L0/Vehicle_Tracking_System/Drivers/BSP/STM32L0538-Discovery" -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-STM32L0538-2d-Discovery

clean-Drivers-2f-BSP-2f-STM32L0538-2d-Discovery:
	-$(RM) ./Drivers/BSP/STM32L0538-Discovery/stm32l0538_discovery.cyclo ./Drivers/BSP/STM32L0538-Discovery/stm32l0538_discovery.d ./Drivers/BSP/STM32L0538-Discovery/stm32l0538_discovery.o ./Drivers/BSP/STM32L0538-Discovery/stm32l0538_discovery.su ./Drivers/BSP/STM32L0538-Discovery/stm32l0538_discovery_epd.cyclo ./Drivers/BSP/STM32L0538-Discovery/stm32l0538_discovery_epd.d ./Drivers/BSP/STM32L0538-Discovery/stm32l0538_discovery_epd.o ./Drivers/BSP/STM32L0538-Discovery/stm32l0538_discovery_epd.su

.PHONY: clean-Drivers-2f-BSP-2f-STM32L0538-2d-Discovery

