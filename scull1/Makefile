obj-m := firstdrv.o
KERNELDIR := /lib/modules/4.8.0-58-generic/build
PWD :=$(shell pwd)
  
modules:  
	$(MAKE)  -C  $(KERNELDIR)  M=$(PWD)  modules
.PHONY :clean
clean:
	rm -rf *.o *ko *order *symvers *mod