CC=g++
route=

all: ./src/main.o ./src/Pipeline/Pipeline.o ./src/ReadImage/ReadImage.o
	$(CC) -Wall -g -std=c++11 ./src/main.o ./src/Pipeline/Pipeline.o ./src/ReadImage/ReadImage.o -o main
	cd src && rm *.o
	cd src/Pipeline && rm *.o
	cd src/ReadImage && rm *.o
	clear

main.o: $(route) ./src/main.cpp
	$(CC) -c $(route)./src/main.cpp

Pipeline.o: $(route) ./src/Pipeline/Pipeline.cpp
	$(CC) -c $(route)./src/Pipeline/Pipeline.cpp

ReadImage.o: $(route) ./src/ReadImage/ReadImage.cpp
	$(CC) -c $(route)./src/ReadImage/ReadImage.cpp