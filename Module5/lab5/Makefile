obj-m += netlink_kernel.o

all: make_kernel make_user
	
clean: clean_kernel clean_user

make_kernel:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean_kernel:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

make_user:
	gcc -o netlink_user netlink_user.c

clean_user:
	rm netlink_user