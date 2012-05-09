################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/YroInternalException.cpp \
../src/core/YroParamHelper.cpp 

OBJS += \
./src/core/YroInternalException.o \
./src/core/YroParamHelper.o 

CPP_DEPS += \
./src/core/YroInternalException.d \
./src/core/YroParamHelper.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/%.o: ../src/core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -I/usr/include/wx-2.8 -I/usr/lib/x86_64-linux-gnu/wx/include/gtk2-unicode-release-2.8 -I"/home/yannick/git/Rakkarack/YroJackGuitar/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


