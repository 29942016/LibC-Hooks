obj-m += kernobject.o
kernobject-objs := mod.o hooks.o io.o


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
		 
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
