CROSS_COMPILE = arm-linux-
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm

STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

export AS LD CC CPP AR NM
export STRIP OBJCOPY OBJDUMP



CFLAGS := -Wall -O2 -g -fno-builtin
#CFLAGS += -I $(shell pwd)/include



LIBFAGS = -lc -L./lib -lgcc -L/usr/local/arm/4.3.2/lib/gcc/arm-none-linux-gnueabi/4.3.2
LDFLAGS := -T myboot.lds 

export CFLAGS LDFLAGS 

TOPDIR := $(shell pwd)
export TOPDIR

TARGET := drew


obj-y += head.o
obj-y += main.o
obj-y += nand.o
obj-y += uart.o
obj-y += timer.o
obj-y += init.o
obj-y += isr.o
obj-y += dma.o
obj-y += interrupt.o
obj-y += fat_app.o
obj-y += i2c/
obj-y += spi/
obj-y += lcd/
obj-y += 1-wire/
obj-y += infrared/
obj-y += lib/
obj-y += sound/
obj-y += fatfs/

all : 
	make -C ./ -f $(TOPDIR)/Makefile.build 
	$(LD) $(LDFLAGS) -o $(TARGET) built-in.o $(LIBFAGS)
	${OBJCOPY} -O binary -S $(TARGET) $(TARGET).bin
	${OBJDUMP} -D -m arm $(TARGET) > $(TARGET).dis
	

clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET)

distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(TARGET)
	