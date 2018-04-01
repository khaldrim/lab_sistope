CC=gcc
route=

all: main.o funciones.o bmp.o
	$(CC) main.o funciones.o bmp.o -o main -Wall -I.
	rm main.o funciones.o bmp.o

main.o: $(route)main.c
	$(CC) -c $(route)main.c

bmp.o: $(route)bmp.c
	$(CC) -c $(route)bmp.c

funciones.o: $(route)funciones.c
	$(CC) -c $(route)funciones.c