# # Gpio Driver Legacy
# BBB_KERNEL := /home/back/bbb_image/bb-kernel/KERNEL
# # TOOLCHAIN := /home/slayder/working_space/BBB/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
# TOOLCHAIN := /home/back/bbb_image/bb-kernel/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-

# EXTRA_CFLAGS=-Wall
# obj-m := hello.o

# all:
# 	sudo make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules
	
# clean:
# 	sudo make -C $(BBB_KERNEL) M=$(shell pwd) clean

# character device driver

EXTRA_CFLAGS = -Wall
obj-m = hello.o

KDIR = /lib/modules/`uname -r`/build

all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` clean
