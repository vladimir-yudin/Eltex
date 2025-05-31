all: build build_libs start

start:
	./main

build:
	gcc -o main main.c -ldl

build_libs: calc_func.o
	gcc -shared calc_func.o -o libmath.so

calc_func.o:
	gcc -fPIC calc_func.c -c

clear:
	rm -rf *.o *.so main