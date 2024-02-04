# Gpio Driver Legacy
BBB_KERNEL := /home/back/bb_kernel/bb-kernel/KERNEL
# TOOLCHAIN := /home/slayder/working_space/BBB/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
TOOLCHAIN := /home/back/bb_kernel/bb-kernel/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
EXTRA_CFLAGS=-Wall
obj-m := module.o

CC := $(TOOLCHAIN)gcc

all:
	make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules
clean:
	make -C $(BBB_KERNEL) M=$(shell pwd) clean