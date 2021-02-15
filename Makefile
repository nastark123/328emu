CC=gcc
CFLAGS=-Wall -I ./include

all: main.o op.o
	$(CC) main.o op.o $(CFLAGS) -o 328emu
	rm -f main.o op.o

main.o:
	$(CC) -c main.c $(CFLAGS) -o main.o

op.o:
	$(CC) -c op.c $(CFLAGS) -o op.o

clean:
	rm -f 328emu

rebuild:
	make clean
	make