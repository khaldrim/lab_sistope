#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "ReadImage.hpp"
#include "../../Bmp/Bmp.hpp"

using namespace std;

ReadImage::ReadImage(){  cout << "Object ReadImage Delete." << endl; }
ReadImage::~ReadImage(){ cout << "Object ReadImage Delete." << endl; }

void ReadImage::main(Bmp *file, int turn){
    readBmpFile(file, turn);
}

void ReadImage::start(int cflag){
    Bmp img[cflag];

    for(int i = 0; i < cflag; i++){
        main(&img[i], i+1);
    }
}

Bmp* ReadImage::readBmpInfoHeader(Bmp *file, FILE *fp){
	file -> setType(fp);
	file -> setFileSize(fp);
	file -> setReserved1(fp);
	file -> setReserved2(fp);
	file -> setOffbits(fp);

	return file;
}

Bmp* ReadImage::readBmpFileHeader(Bmp *file, FILE *fp){
	file -> setSize(fp);
	file -> setWidth(fp);
	file -> setHeight(fp);
    file -> setPlanes(fp);
    file -> setBitPerPixel(fp);
    file -> setCompression(fp);
    file -> setSizeImage(fp);
    file -> setxPelsPerMeter(fp);
    file -> setyPelsPerMeter(fp);
    file -> setUsed(fp);
    file -> setImportant(fp);
    file -> setRedMask(fp);
    file -> setGreenMask(fp);
    file -> setBlueMask(fp);
    file -> setAlphaMask(fp);
    file -> setCsType(fp);
    file -> setXRed(fp);
    file -> setYRed(fp);
    file -> setZRed(fp);
    file -> setXGreen(fp);
    file -> setYGreen(fp);
    file -> setZGreen(fp);
    file -> setXBlue(fp);
    file -> setYBlue(fp);
    file -> setZBlue(fp);
    file -> setGammaRed(fp);
    file -> setGammaGreen(fp);
    file -> setGammaBlue(fp);
    file -> setIntent(fp);
    file -> setProfileData(fp);
    file -> setProfileSize(fp);
    file -> setReserved(fp);

    return file;
}

void ReadImage::readBmpFile(Bmp *file, int img){

    FILE *fp;

    char fileNumber[10];
    char fileName[50] = "./img/imagen_";

    sprintf(fileNumber, "%d", img);
    strcat(fileName, fileNumber);
    strcat(fileName, ".bmp");

    cout << "fileName: " << fileName << endl;
    
    if((fp = fopen(fileName, "rb")) == NULL)
    {
        printf("No se logro abrir el archivo: %s.\n", fileName);
        exit(1);
    }
    

	this -> ReadImage::readBmpInfoHeader(file, fp);
	this -> ReadImage::readBmpFileHeader(file, fp);

    cout << "termino de leer." << endl; 
}