################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/YroJackGuitar.cpp 

C_UPPER_SRCS += \
../src/Echo.C 

OBJS += \
./src/Echo.o \
./src/YroJackGuitar.o 

CPP_DEPS += \
./src/YroJackGuitar.d 

C_UPPER_DEPS += \
./src/Echo.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.C
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -I/usr/include/wx-2.8 -I/usr/lib/x86_64-linux-gnu/wx/include/gtk2-unicode-release-2.8 -I"/home/yannick/git/Rakkarack/YroJackGuitar/src" -O0 -msse2 -mfpmath=sse  -ffast-math -pipe -fsigned-char -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -D_THREAD_SAFE -D_REENTRANT -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -I/usr/include/wx-2.8 -I/usr/lib/x86_64-linux-gnu/wx/include/gtk2-unicode-release-2.8 -I"/home/yannick/git/Rakkarack/YroJackGuitar/src" -O0 -msse2 -mfpmath=sse  -ffast-math -pipe -fsigned-char -fstack-protector --param=ssp-buffer-size=4 -Wformat -Wformat-security -D_THREAD_SAFE -D_REENTRANT -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


