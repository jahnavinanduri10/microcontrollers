################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/BSP/font12epd.c \
../Core/BSP/font16epd.c \
../Core/BSP/font20epd.c \
../Core/BSP/font8epd.c \
../Core/BSP/stm32l0538_discovery.c \
../Core/BSP/stm32l0538_discovery_epd.c 

OBJS += \
./Core/BSP/font12epd.o \
./Core/BSP/font16epd.o \
./Core/BSP/font20epd.o \
./Core/BSP/font8epd.o \
./Core/BSP/stm32l0538_discovery.o \
./Core/BSP/stm32l0538_discovery_epd.o 

C_DEPS += \
./Core/BSP/font12epd.d \
./Core/BSP/font16epd.d \
./Core/BSP/font20epd.d \
./Core/BSP/font8epd.d \
./Core/BSP/stm32l0538_discovery.d \
./Core/BSP/stm32l0538_discovery_epd.d 


# Each subdirectory must supply rules for building sources it contributes
Core/BSP/%.o Core/BSP/%.su Core/BSP/%.cyclo: ../Core/BSP/%.c Core/BSP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L053xx -c -I../Core/Inc -I"D:/stm32L0/first/Core/BSP" -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-BSP

clean-Core-2f-BSP:
	-$(RM) ./Core/BSP/font12epd.cyclo ./Core/BSP/font12epd.d ./Core/BSP/font12epd.o ./Core/BSP/font12epd.su ./Core/BSP/font16epd.cyclo ./Core/BSP/font16epd.d ./Core/BSP/font16epd.o ./Core/BSP/font16epd.su ./Core/BSP/font20epd.cyclo ./Core/BSP/font20epd.d ./Core/BSP/font20epd.o ./Core/BSP/font20epd.su ./Core/BSP/font8epd.cyclo ./Core/BSP/font8epd.d ./Core/BSP/font8epd.o ./Core/BSP/font8epd.su ./Core/BSP/stm32l0538_discovery.cyclo ./Core/BSP/stm32l0538_discovery.d ./Core/BSP/stm32l0538_discovery.o ./Core/BSP/stm32l0538_discovery.su ./Core/BSP/stm32l0538_discovery_epd.cyclo ./Core/BSP/stm32l0538_discovery_epd.d ./Core/BSP/stm32l0538_discovery_epd.o ./Core/BSP/stm32l0538_discovery_epd.su

.PHONY: clean-Core-2f-BSP

