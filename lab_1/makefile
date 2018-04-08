CC=gcc
route=

all: main.o function.o bmp.o
	$(CC) main.o function.o bmp.o -o main -Wall -I.
	rm main.o function.o bmp.o

main.o: $(route)main.c
	$(CC) -c $(route)main.c

bmp.o: $(route)bmp.c
	$(CC) -c $(route)bmp.c

function.o: $(route)function.c
	$(CC) -c $(route)function.c