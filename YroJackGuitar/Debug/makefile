################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/wx/custom/subdir.mk
-include src/wx/subdir.mk
-include src/utils/subdir.mk
-include src/tests/effects/subdir.mk
-include src/plugins/noisegate/subdir.mk
-include src/plugins/misc/subdir.mk
-include src/plugins/filter/subdir.mk
-include src/plugins/effect/subdir.mk
-include src/plugins/distortion/subdir.mk
-include src/plugins/analyze/subdir.mk
-include src/plugins/ampli/subdir.mk
-include src/plugins/subdir.mk
-include src/jack/subdir.mk
-include src/common/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: YroJackGuitar

# Tool invocations
YroJackGuitar: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "YroJackGuitar" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) YroJackGuitar
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
