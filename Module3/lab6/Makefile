all: build

build: libmsg.a
	gcc ping.c -o ping -L. -lmsg
	gcc pong.c -o pong -L. -lmsg

libmsg.a:
	gcc lib.c -c
	ar rc libmsg.a lib.o

clean:
	rm -rf *.o ping pong libmsg.a
