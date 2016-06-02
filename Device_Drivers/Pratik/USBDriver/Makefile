# If called directly from the command line, invoke the kernel build system.
ifeq ($(KERNELRELEASE),)

	KERNEL_SOURCE := /home/pratik/Project/LCDK_Training/LCDK_Board/pratik/SDKS/ti-sdk-omapl138-lcdk-01.00.00/board-support/linux-3.1.10
	PWD := $(shell pwd)
	ARCH :=

	CROSS_COMPILE :=

	MAKEARCH := $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)

default:
	$(MAKEARCH) -C $(KERNEL_SOURCE) SUBDIRS=$(PWD) modules

clean:
	$(MAKEARCH) -C $(KERNEL_SOURCE) SUBDIRS=$(PWD) clean

# Otherwise KERNELRELEASE is defined; we've been invoked from the
# kernel build system and can use its language.
else

	src-m := $(notdir $(wildcard ${SUBDIRS}/*.c))
	src-mod := $(notdir $(wildcard ${SUBDIRS}/*.mod.c))
	src-m := $(filter-out ${src-mod}, ${src-m})
	obj-m := $(src-m:.c=.o)

endif
