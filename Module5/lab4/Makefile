obj-m += chardev.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

load: all
	sudo insmod $(basename $(obj-m)).ko

unload: clean
	sudo rmmod $(basename $(obj-m))