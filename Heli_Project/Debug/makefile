################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./buttons4.obj" \
"./circBufT.obj" \
"./main.obj" \
"./rotors.obj" \
"./tm4c123gh6pm_startup_ccs.obj" \
"./ustdlib.obj" \
"./yaw.obj" \
"./OrbitOLED/OrbitOLEDInterface.obj" \
"./OrbitOLED/lib_OrbitOled/ChrFont0.obj" \
"./OrbitOLED/lib_OrbitOled/FillPat.obj" \
"./OrbitOLED/lib_OrbitOled/OrbitOled.obj" \
"./OrbitOLED/lib_OrbitOled/OrbitOledChar.obj" \
"./OrbitOLED/lib_OrbitOled/OrbitOledGrph.obj" \
"./OrbitOLED/lib_OrbitOled/delay.obj" \
"../tm4c123gh6pm.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \
-l"C:/ti/TivaWare_C_Series-2.2.0.295/driverlib/ccs/Debug/driverlib.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include OrbitOLED/subdir_vars.mk
-include OrbitOLED/lib_OrbitOled/subdir_vars.mk
-include subdir_rules.mk
-include OrbitOLED/subdir_rules.mk
-include OrbitOLED/lib_OrbitOled/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Heli_Project.out \

EXE_OUTPUTS__QUOTED += \
"Heli_Project.out" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "Heli_Project.out"

# Tool invocations
Heli_Project.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi -z -m"Heli_Project.map" --heap_size=256 --stack_size=512 -i"C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/lib" -i"C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="Heli_Project_linkInfo.xml" --rom_model -o "Heli_Project.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "buttons4.obj" "circBufT.obj" "main.obj" "rotors.obj" "tm4c123gh6pm_startup_ccs.obj" "ustdlib.obj" "yaw.obj" "OrbitOLED\OrbitOLEDInterface.obj" "OrbitOLED\lib_OrbitOled\ChrFont0.obj" "OrbitOLED\lib_OrbitOled\FillPat.obj" "OrbitOLED\lib_OrbitOled\OrbitOled.obj" "OrbitOLED\lib_OrbitOled\OrbitOledChar.obj" "OrbitOLED\lib_OrbitOled\OrbitOledGrph.obj" "OrbitOLED\lib_OrbitOled\delay.obj" 
	-$(RM) "buttons4.d" "circBufT.d" "main.d" "rotors.d" "tm4c123gh6pm_startup_ccs.d" "ustdlib.d" "yaw.d" "OrbitOLED\OrbitOLEDInterface.d" "OrbitOLED\lib_OrbitOled\ChrFont0.d" "OrbitOLED\lib_OrbitOled\FillPat.d" "OrbitOLED\lib_OrbitOled\OrbitOled.d" "OrbitOLED\lib_OrbitOled\OrbitOledChar.d" "OrbitOLED\lib_OrbitOled\OrbitOledGrph.d" "OrbitOLED\lib_OrbitOled\delay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

