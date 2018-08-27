#include <iostream>
#include <string.h>
#include "ReadImage.hpp"
#include "../../Bmp/Bmp.hpp"

using namespace std;


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

Bmp* ReadImage::readBmpFile(Bmp *img, int img){

	FILE *fp;
    char fileNumber[5];
    char fileName[35] = "imagenes/imagen_";

    sprintf(fileNumber, "%d", img);
    strcat(fileName, fileNumber);
    strcat(fileName, ".bmp");

    if((fp = fopen(fileName,"rb")) == NULL)
    {
        printf("No se logro abrir el archivo: %s.\n", fileName);
        exit(1);
    }

	this -> readBmpInfoHeader(img);
	this -> readBmpFileHeader(img);

	cout << "El largo de la imagen es: " << img -> getWidth() << "\n";

	return img;
}