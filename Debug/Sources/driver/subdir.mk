################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Sources/driver/adc.cpp \
../Sources/driver/ftm.cpp \
../Sources/driver/pit.cpp 

OBJS += \
./Sources/driver/adc.o \
./Sources/driver/ftm.o \
./Sources/driver/pit.o 

CPP_DEPS += \
./Sources/driver/adc.d \
./Sources/driver/ftm.d \
./Sources/driver/pit.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/driver/%.o: ../Sources/driver/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"../Sources" -I"H:\KDSWorkSpace\KEA128\Includes\driver" -I"../Includes" -std=gnu++11 -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


