obj-m += netlink_kernel.o

all: make_kernel make_user
	
clean: make_kernel_clean make_user_clean

make_kernel:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

make_kernel_clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

make_user:
	gcc -o netlink_user netlink_user.c

make_user_clean:
	rm netlink_user