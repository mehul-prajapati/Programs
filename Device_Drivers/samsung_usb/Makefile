####################################
# Makefile for Device Driver
# Author: Mehul Prajapati 
# <mehulprajapati280@gmail.com>
####################################

# if KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq (${KERNELRELEASE},)
	obj-m:= samsungUsb.o

# Otherwise we were called directly from the command line.
# Invoke the kernel build system.
else
    KERNEL_SOURCE := /usr/src/linux-headers-`uname -r`
    PWD := $(shell pwd)

all:
	${MAKE} -C ${KERNEL_SOURCE} M=${PWD} modules

clean:
	${MAKE} -C ${KERNEL_SOURCE} M=${PWD} clean

endif
