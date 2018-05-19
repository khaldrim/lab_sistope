CC=gcc
route=

all: main.o readImage.o bmp.o scaleGray.o
	$(CC) main.o -o main -Wall -I.
	$(CC) readImage.o bmp.o -o readImage -Wall -I.
	$(CC) scaleGray.o -o scaleGray -Wall -I.
	$(CC) binaryImage.c -o binaryImage -Wall -I.
	$(CC) analisisImage.c -o analisisImage -Wall -I.
	$(CC) writeImage.c -o writeImage -Wall -I.
	rm *.o

main.o: $(route)main.c
	$(CC) -c $(route)main.c

readImage.o: $(route)readImage.c
	$(CC) -c $(route)readImage.c

bmp.o: $(route)bmp.c
	$(CC) -c $(route)bmp.c

scaleGray.o: $(route)scaleGray.c
	$(CC) -c $(route)scaleGray.c

binaryImage.o: $(route)binaryImage.c
	$(CC) -c $(route)binaryImage.c

analisisImage.o: $(route)analisisImage.c
	$(CC) -c $(route)analisisImage.c

writeImage.o: $(route)writeImage.c
	$(CC) -c $(route)writeImage.c