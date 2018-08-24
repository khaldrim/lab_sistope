CC=../u++-7.0.0/bin/u++
route=

all: ./src/main.o ./src/Pipeline/Pipeline.o ./src/Pipeline/ReadImage/ReadImage.o ./src/Pipeline/GreyData/GreyData.o ./src/Pipeline/NearlyBlack/NearlyBlack.o ./src/Pipeline/WriteImage/WriteImage.o ./src/Pipeline/BinData/BinData.o
	$(CC) -Wall -g -std=c++11 ./src/main.o ./src/Pipeline/Pipeline.o ./src/Pipeline/ReadImage/ReadImage.o ./src/Pipeline/GreyData/GreyData.o ./src/Pipeline/NearlyBlack/NearlyBlack.o ./src/Pipeline/WriteImage/WriteImage.o ./src/Pipeline/BinData/BinData.o -o main
	cd src && rm *.o
	cd src/Pipeline && rm *.o
	cd src/Pipeline/ReadImage && rm *.o
	cd src/Pipeline/GreyData && rm *.o
	cd src/Pipeline/NearlyBlack && rm *.o
	cd src/Pipeline/WriteImage && rm *.o
	cd src/Pipeline/BinData && rm *.o
	clear

main.o: $(route) ./src/main.cpp
	$(CC) -c $(route)./src/main.cpp

Pipeline.o: $(route) ./src/Pipeline/Pipeline.cpp
	$(CC) -c $(route)./src/Pipeline/Pipeline.cpp

ReadImage.o: $(route) ./src/Pipeline/ReadImage/ReadImage.cpp
	$(CC) -c $(route)./src/Pipeline/ReadImage/ReadImage.cpp

GreyData.o: $(route) ./src/Pipeline/GreyData/GreyData.cpp
	$(CC) -c $(route)./src/Pipeline/GreyData/GreyData.cpp

NearlyBlack.o: $(route) ./src/Pipeline/NearlyBlack/NearlyBlack.cpp
	$(CC) -c $(route)./src/Pipeline/NearlyBlack/NearlyBlack.cpp

WriteImage.o: $(route) ./src/Pipeline/WriteImage/WriteImage.cpp
	$(CC) -c $(route)./src/Pipeline/WriteImage/WriteImage.cpp

BinData.o: $(route) ./src/Pipeline/BinData/BinData.cpp
	$(CC) -c $(route)./src/Pipeline/BinData/BinData.cpp