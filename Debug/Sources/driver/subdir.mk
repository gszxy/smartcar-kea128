################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Sources/driver/UART.cpp \
../Sources/driver/adc.cpp \
../Sources/driver/ftm.cpp \
../Sources/driver/gpio.cpp \
../Sources/driver/kbi.cpp \
../Sources/driver/pit.cpp 

OBJS += \
./Sources/driver/UART.o \
./Sources/driver/adc.o \
./Sources/driver/ftm.o \
./Sources/driver/gpio.o \
./Sources/driver/kbi.o \
./Sources/driver/pit.o 

CPP_DEPS += \
./Sources/driver/UART.d \
./Sources/driver/adc.d \
./Sources/driver/ftm.d \
./Sources/driver/gpio.d \
./Sources/driver/kbi.d \
./Sources/driver/pit.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/driver/%.o: ../Sources/driver/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"../Sources" -I"H:\KDSWorkSpace\KEA128\Includes\utility" -I"H:\KDSWorkSpace\KEA128\Includes\driver" -I"../Includes" -std=gnu++11 -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


