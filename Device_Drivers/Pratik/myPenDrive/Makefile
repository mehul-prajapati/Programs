#KDIR := /lib/modules/$(shell uname -r)/build/

KDIR := /home/pratik/Project/LCDK_Training/LCDK_Board/pratik/SDKS/ti-sdk-omapl138-lcdk-01.00.00/board-support/linux-3.1.10

ARCH :=

CROSS_COMPILE :=
obj-m += usbPenDriver.o

PWD := $(shell pwd)

MAKEARCH := $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)

default:
	$(MAKEARCH) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKEARCH) -C $(KDIR) M=$(PWD) clean
