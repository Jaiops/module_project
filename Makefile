ifneq ($(KERNELRELEASE),)
	obj-m := test.o
	obj-m += lkm_writer.o
else

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
#KERNELDIR ?= /home/nitin/Desktop/src/linux-3.11.1/

PWD := $(shell pwd)

default:
		$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
clean:
		$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

