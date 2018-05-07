CC=gcc
route=

all: main.o readImage.o 
	$(CC) main.o -o main -Wall -I.
	$(CC) readImage.o -o readImage -Wall -I.
	rm main.o readImage.o 

main.o: $(route)main.c
	$(CC) -c $(route)main.c

# bmp.o: $(route)bmp.c
# 	$(CC) -c $(route)bmp.c

readImage.o: $(route)readImage.c
	$(CC) -c $(route)readImage.c

# scaleGray.o: $(route)scaleGray.c
# 	$(CC) -c $(route)scaleGray.c