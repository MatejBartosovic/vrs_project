################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/GenericSPI.cpp \
../src/RaspberryPiSPI.cpp \
../src/pi_rf22.cpp 

OBJS += \
./src/GenericSPI.o \
./src/RaspberryPiSPI.o \
./src/pi_rf22.o 

CPP_DEPS += \
./src/GenericSPI.d \
./src/RaspberryPiSPI.d \
./src/pi_rf22.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


