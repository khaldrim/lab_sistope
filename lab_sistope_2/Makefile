CC=gcc
route=

all: main.o readImage.o bmp.o scaleGray.o
	$(CC) main.o -o main -Wall -I.
	$(CC) readImage.o bmp.o -o readImage -Wall -I.
	$(CC) scaleGray.o -o scaleGray -Wall -I.
	rm main.o readImage.o bmp.o scaleGray.o

main.o: $(route)main.c
	$(CC) -c $(route)main.c

readImage.o: $(route)readImage.c
	$(CC) -c $(route)readImage.c

bmp.o: $(route)bmp.c
	$(CC) -c $(route)bmp.c

scaleGray.o: $(route)scaleGray.c
	$(CC) -c $(route)scaleGray.c