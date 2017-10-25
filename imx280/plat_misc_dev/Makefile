obj-m := my_plat_miscdev.o
KERNELDIR := /home/qing/imx280/linux-2.6.35.3
PWD :=$(shell pwd)
  
modules:  
	$(MAKE)  -C  $(KERNELDIR)  M=$(PWD)  modules
.PHONY :clean
clean:
	rm -rf *.o *ko *order *symvers *mod
