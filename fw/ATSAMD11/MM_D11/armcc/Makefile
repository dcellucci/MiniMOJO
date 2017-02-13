
################################################################################
# Automatically-generated file. Do not edit!
################################################################################

ifdef SystemRoot
	SHELL = cmd.exe
	MK_DIR = mkdir
else
	ifeq ($(shell uname), Linux)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), CYGWIN)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW32)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW64)
		MK_DIR = mkdir -p
	endif
endif

# List the subdirectories for creating object files
SUB_DIRS +=  \
 \
hpl/port \
hpl/dmac \
hpl/tcc \
hal/src \
hpl/pm \
hpl/sysctrl \
hal/utils/src \
hpl/sercom \
examples \
hpl/gclk \
armcc/Device/SAMD11/Source/ARM \
hpl/core \
armcc/Device/SAMD11/Source

# List the object files
OBJS +=  \
driver_init.o \
examples/driver_examples.o \
hal/src/hal_pwm.o \
hpl/core/hpl_core_m0plus_base.o \
hpl/core/hpl_init.o \
hpl/dmac/hpl_dmac.o \
hpl/gclk/hpl_gclk.o \
hpl/pm/hpl_pm.o \
hpl/port/hpl_port.o \
hpl/sercom/hpl_sercom.o \
hpl/sysctrl/hpl_sysctrl.o \
hpl/tcc/hpl_tcc.o \
atmel_start.o \
hal/src/hal_atomic.o \
hal/src/hal_delay.o \
hal/src/hal_gpio.o \
hal/src/hal_i2c_m_sync.o \
hal/src/hal_i2c_s_sync.o \
hal/src/hal_init.o \
hal/src/hal_io.o \
hal/src/hal_sleep.o \
hal/utils/src/utils_assert.o \
hal/utils/src/utils_event.o \
hal/utils/src/utils_list.o \
main.o \
armcc/Device/SAMD11/Source/ARM/startup_SAMD11.o \
armcc/Device/SAMD11/Source/system_samd11.o

OBJS_AS_ARGS +=  \
"driver_init.o" \
"examples/driver_examples.o" \
"hal/src/hal_pwm.o" \
"hpl/core/hpl_core_m0plus_base.o" \
"hpl/core/hpl_init.o" \
"hpl/dmac/hpl_dmac.o" \
"hpl/gclk/hpl_gclk.o" \
"hpl/pm/hpl_pm.o" \
"hpl/port/hpl_port.o" \
"hpl/sercom/hpl_sercom.o" \
"hpl/sysctrl/hpl_sysctrl.o" \
"hpl/tcc/hpl_tcc.o" \
"atmel_start.o" \
"hal/src/hal_atomic.o" \
"hal/src/hal_delay.o" \
"hal/src/hal_gpio.o" \
"hal/src/hal_i2c_m_sync.o" \
"hal/src/hal_i2c_s_sync.o" \
"hal/src/hal_init.o" \
"hal/src/hal_io.o" \
"hal/src/hal_sleep.o" \
"hal/utils/src/utils_assert.o" \
"hal/utils/src/utils_event.o" \
"hal/utils/src/utils_list.o" \
"main.o" \
"armcc/Device/SAMD11/Source/ARM/startup_SAMD11.o" \
"armcc/Device/SAMD11/Source/system_samd11.o"

# List the dependency files
DEPS := $(OBJS:%.o=%.d)

DEPS_AS_ARGS +=  \
"driver_init.d" \
"examples/driver_examples.d" \
"hal/src/hal_pwm.d" \
"hpl/core/hpl_core_m0plus_base.d" \
"hpl/core/hpl_init.d" \
"hpl/dmac/hpl_dmac.d" \
"hpl/gclk/hpl_gclk.d" \
"hpl/pm/hpl_pm.d" \
"hpl/port/hpl_port.d" \
"hpl/sercom/hpl_sercom.d" \
"hpl/sysctrl/hpl_sysctrl.d" \
"hpl/tcc/hpl_tcc.d" \
"atmel_start.d" \
"hal/src/hal_atomic.d" \
"hal/src/hal_delay.d" \
"hal/src/hal_gpio.d" \
"hal/src/hal_i2c_m_sync.d" \
"hal/src/hal_i2c_s_sync.d" \
"hal/src/hal_init.d" \
"hal/src/hal_io.d" \
"hal/src/hal_sleep.d" \
"hal/utils/src/utils_assert.d" \
"hal/utils/src/utils_event.d" \
"hal/utils/src/utils_list.d" \
"main.d" \
"armcc/Device/SAMD11/Source/ARM/startup_SAMD11.d" \
"armcc/Device/SAMD11/Source/system_samd11.d"

OUTPUT_FILE_NAME :=AtmelStart
QUOTE := "
OUTPUT_FILE_PATH +=$(OUTPUT_FILE_NAME).elf
OUTPUT_FILE_PATH_AS_ARGS +=$(OUTPUT_FILE_NAME).elf

vpath %.c ../
vpath %.s ../
vpath %.S ../

# All Target
all: $(SUB_DIRS) $(OUTPUT_FILE_PATH)

# Linker target

$(OUTPUT_FILE_PATH): $(OBJS)
	@echo Building target: $@
	@echo Invoking: ARMCC Linker
	$(QUOTE)armlink$(QUOTE) --ro-base 0x00000000 --entry 0x00000000 --rw-base 0x20000000 --entry Reset_Handler --first __Vectors \
--strict --summary_stderr --info summarysizes --map --xref --callgraph --symbols \
--info sizes --info totals --info unused --info veneers --list $(OUTPUT_FILE_NAME).map \
-o $(OUTPUT_FILE_NAME).elf --cpu Cortex-M0+ \
$(OBJS_AS_ARGS)

	@echo Finished building target: $@

# Compiler target(s)




%.o: %.c
	@echo Building file: $<
	@echo ARMCC Compiler
	$(QUOTE)armcc$(QUOTE) --c99 -c -DDEBUG -O1 -g --apcs=interwork --split_sections --cpu Cortex-M0+ -D__SAMD11D14AS__ \
-I"../" -I"../config" -I"../examples" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/core" -I"../hpl/dmac" -I"../hpl/gclk" -I"../hpl/pm" -I"../hpl/port" -I"../hpl/sercom" -I"../hpl/sysctrl" -I"../hpl/tcc" -I"../hri" -I"../" -I"../CMSIS/Include" -I"../include"  \
--depend "$@"  -o "$@" "$<"

	@echo Finished building: $<

%.o: %.s
	@echo Building file: $<
	@echo ARMCC Assembler
	$(QUOTE)armasm$(QUOTE) -g --apcs=interwork --cpu Cortex-M0+ --pd "D__SAMD11D14AS__ SETA 1" \
-I"../" -I"../config" -I"../examples" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/core" -I"../hpl/dmac" -I"../hpl/gclk" -I"../hpl/pm" -I"../hpl/port" -I"../hpl/sercom" -I"../hpl/sysctrl" -I"../hpl/tcc" -I"../hri" -I"../" -I"../CMSIS/Include" -I"../include"  \
--depend "$(@:%.o=%.d)" -o "$@" "$<"

	@echo Finished building: $<

%.o: %.S
	@echo Building file: $<
	@echo ARMCC Preprocessing Assembler
	$(QUOTE)armcc$(QUOTE) --c99 -c -DDEBUG -O1 -g --apcs=interwork --split_sections --cpu Cortex-M0+ -D__SAMD11D14AS__ \
-I"../" -I"../config" -I"../examples" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/core" -I"../hpl/dmac" -I"../hpl/gclk" -I"../hpl/pm" -I"../hpl/port" -I"../hpl/sercom" -I"../hpl/sysctrl" -I"../hpl/tcc" -I"../hri" -I"../" -I"../CMSIS/Include" -I"../include"  \
--depend "$@"  -o "$@" "$<"

	@echo Finished building: $<

# Detect changes in the dependent files and recompile the respective object files.
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif
endif

$(SUB_DIRS):
	$(MK_DIR) "$@"

clean:
	rm -f $(OBJS_AS_ARGS)
	rm -f $(OUTPUT_FILE_PATH)
	rm -f $(DEPS_AS_ARGS)
	rm -f $(OUTPUT_FILE_NAME).map $(OUTPUT_FILE_NAME).elf
