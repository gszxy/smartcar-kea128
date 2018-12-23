################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Sources/cppmain.cpp \
../Sources/glb_var.cpp \
../Sources/intr.cpp \
../Sources/state_machine.cpp 

C_SRCS += \
../Sources/main.c 

OBJS += \
./Sources/cppmain.o \
./Sources/glb_var.o \
./Sources/intr.o \
./Sources/main.o \
./Sources/state_machine.o 

C_DEPS += \
./Sources/main.d 

CPP_DEPS += \
./Sources/cppmain.d \
./Sources/glb_var.d \
./Sources/intr.d \
./Sources/state_machine.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"../Sources" -I"H:\KDSWorkSpace\KEA128\Includes\utility" -I"H:\KDSWorkSpace\KEA128\Includes\driver" -I"../Includes" -std=gnu++11 -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"../Sources" -I"../Includes" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


