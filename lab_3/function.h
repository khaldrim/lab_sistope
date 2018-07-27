#include "struct.h"

#ifndef _FUNCTION_H_
#define _FUNCTION_H_

//function file
int mainMenu(int cflag, int hflag, int uflag, int nflag, int bflag);
unsigned char** readBMPImage(int imgCount, BITMAPFILEHEADER* bmpFileHeader, BITMAPINFOHEADER* bmpInfoHeader);
unsigned char** createBuffer(int width, int height);
DATA* initializeData(DATA *data, int totalSize);
void grayData(DATA *data, int height, int width);
int checkPixelData(DATA *data, int width, int height);
void binaryData(DATA *data, INPUTDATA* inputData, int width, int height);
int checkBinData(DATA *data, int width, int height);
void writeBinaryImage(DATA* data, INPUTDATA* inputData, BITMAPFILEHEADER* bmpFileHeader, BITMAPINFOHEADER* bmpInfoHeader);

//BMP file
BITMAPFILEHEADER *ReadBMPFileHeader(FILE *fp, BITMAPFILEHEADER  *header);
BITMAPINFOHEADER *ReadBMPInfoHeader(FILE *fp, BITMAPINFOHEADER *header);
unsigned short ReadLE2(FILE *fp);
unsigned int ReadLE4(FILE *fp);
unsigned int ReadLE8(FILE *fp);

#endif
