CC=gcc
route=

all: main.o funciones.o
	$(CC) main.o funciones.o -o main -Wall -I.
	rm main.o funciones.o

main.o: $(route)main.c
	$(CC) -c $(route)main.c

funciones.o: $(route)funciones.c
	$(CC) -c $(route)funciones.c