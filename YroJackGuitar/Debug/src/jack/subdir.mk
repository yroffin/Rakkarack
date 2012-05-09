################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/jack/YroAudioSampleFactory.cpp \
../src/jack/YroEffectFactory.cpp \
../src/jack/YroJackDriver.cpp 

OBJS += \
./src/jack/YroAudioSampleFactory.o \
./src/jack/YroEffectFactory.o \
./src/jack/YroJackDriver.o 

CPP_DEPS += \
./src/jack/YroAudioSampleFactory.d \
./src/jack/YroEffectFactory.d \
./src/jack/YroJackDriver.d 


# Each subdirectory must supply rules for building sources it contributes
src/jack/%.o: ../src/jack/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -I/usr/include/wx-2.8 -I/usr/lib/x86_64-linux-gnu/wx/include/gtk2-unicode-release-2.8 -I"/home/yannick/git/Rakkarack/YroJackGuitar/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


