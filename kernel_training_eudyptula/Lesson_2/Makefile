
KERNEL_DIR = /usr/src/linux-headers-`uname -r`
obj-m := hello.o

PWD = `pwd`

all:
	$(MAKE) -C $(KERNEL_DIR) M=${PWD} modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=${PWD} clean 
	
