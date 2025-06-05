obj-m += chardev.o

all: make_chardev make_test
	
clean: clean_chardev clean_test

make_chardev:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean_chardev:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

make_test:
	gcc -o netlink_user netlink_user.c

clean_test:
	rm netlink_user