all: build_libs build
	./main

build:
	gcc -o main main.c -ldl

order_list.o: order_list.c
	gcc -fPIC order_list.c -c

build_libs: order_list.o
	gcc -shared order_list.o -o liblist.so

clear:
	rm -rf *.o *.so main