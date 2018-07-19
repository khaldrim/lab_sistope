#include "struct.h"

#ifndef _FUNCTION_H_
#define _FUNCTION_H_

//function file
int mainMenu(int cflag, int hflag, int uflag, int nflag, int bflag);
unsigned char** readBMPImage(int imgCount, BITMAPFILEHEADER* bmpFileHeader, BITMAPINFOHEADER* bmpInfoHeader);
unsigned char** createBuffer(int width, int height, int bitPerPixel);

//BMP file
BITMAPFILEHEADER *ReadBMPFileHeader(FILE *fp, BITMAPFILEHEADER  *header);
BITMAPINFOHEADER *ReadBMPInfoHeader(FILE *fp, BITMAPINFOHEADER *header);
unsigned short ReadLE2(FILE *fp);
unsigned int ReadLE4(FILE *fp);
unsigned int ReadLE8(FILE *fp);

#endif