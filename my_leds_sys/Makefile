obj-m := my_led_ledsys.o
KERNELDIR := /home/qing/iTop4412_Kernel_3.0
PWD :=$(shell pwd)
  
modules:  
	$(MAKE)  -C  $(KERNELDIR)  M=$(PWD)  modules
.PHONY :clean
clean:
	rm -rf *.o *ko *order *symvers *mod
