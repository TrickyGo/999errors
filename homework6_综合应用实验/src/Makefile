#sample driver module
    obj-m := scull.o
    KDIR = /root/linux-4.15.0/
    PWD:=$(shell pwd)
    INSTALLDIR=$(PWD)
modules:
	$(MAKE) -C /lib/modules/`uname -r`/build M=`pwd` modules
clean:
	rm -f *.mod.c *.mod.o *.ko *.o *.tmp_versions
.PHONY:
	modules clean
