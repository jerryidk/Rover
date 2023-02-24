TARGET = main

# Define the linker script location and chip architecture.
LD_SCRIPT = STM32F072.ld
MCU_SPEC  = cortex-m0

# Toolchain definitions (ARM bare metal defaults)
TOOLCHAIN = /usr
CC = $(TOOLCHAIN)/bin/arm-none-eabi-gcc
AS = $(TOOLCHAIN)/bin/arm-none-eabi-as
LD = $(TOOLCHAIN)/bin/arm-none-eabi-ld
OC = $(TOOLCHAIN)/bin/arm-none-eabi-objcopy
OD = $(TOOLCHAIN)/bin/arm-none-eabi-objdump
OS = $(TOOLCHAIN)/bin/arm-none-eabi-size

# Assembly directives.
ASFLAGS = -c -O0 -mcpu=$(MCU_SPEC) -mthumb -Wall -fmessage-length=0

# C compilation directives
CFLAGS = -mcpu=$(MCU_SPEC) -mthumb -Wall -g -fmessage-length=0 --specs=nosys.specs

# Linker directives
LSCRIPT = ./ld/$(LD_SCRIPT)
LFLAGS += -mcpu=$(MCU_SPEC) -mthumb -Wall --specs=nosys.specs -nostdlib -lgcc -T$(LSCRIPT)

AS_SRC   = ./src/boot.s
C_SRC    =  $(wildcard ./src/*.c)
INCLUDE  =  -I./ -I./device_headers
OBJS  = $(AS_SRC:.s=.o)
OBJS += $(C_SRC:.c=.o)

.PHONY: all clean 

all: $(TARGET).bin 

%.o: %.s 
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

%.o: %.c 
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OC) -S -O binary $< $@
	$(OS) $<


flash: $(TARGET).bin
	st-flash write $(TARGET).bin 0x8000000

clean:
	rm -f $(OBJS)
	rm -f $(TARGET).elf
	rm -f $(TARGET).bin
