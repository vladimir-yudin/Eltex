all: liborderlist.a
	gcc -o main main.c -L. -lorderlist
	./main
	
order_list.o: order_list.c
	gcc order_list.c -c

liborderlist.a: order_list.o
	ar rc liborderlist.a order_list.o

clean: 
	rm -rf *.o *.a main

