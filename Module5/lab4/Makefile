obj-m += chardev.o

all: test
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean: test_clean
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	
test: test.c
	cc -o test test.c

test_clean:
	rm test