################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Sources/utility/communicate.cpp \
../Sources/utility/motor.cpp \
../Sources/utility/nueron.cpp \
../Sources/utility/pid.cpp \
../Sources/utility/sensor.cpp \
../Sources/utility/steer.cpp 

OBJS += \
./Sources/utility/communicate.o \
./Sources/utility/motor.o \
./Sources/utility/nueron.o \
./Sources/utility/pid.o \
./Sources/utility/sensor.o \
./Sources/utility/steer.o 

CPP_DEPS += \
./Sources/utility/communicate.d \
./Sources/utility/motor.d \
./Sources/utility/nueron.d \
./Sources/utility/pid.d \
./Sources/utility/sensor.d \
./Sources/utility/steer.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/utility/%.o: ../Sources/utility/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"../Sources" -I"H:\KDSWorkSpace\KEA128\Includes\utility" -I"H:\KDSWorkSpace\KEA128\Includes\driver" -I"../Includes" -std=gnu++11 -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


