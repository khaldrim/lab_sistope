#include "struct.h"

#ifndef _FUNCIONES_H_
#define _FUNCIONES_H_

/* bmp.c file */
BITMAPFILEHEADER *ReadBMPFileHeader(FILE *fp, BITMAPFILEHEADER  *header);
BITMAPINFOHEADER *ReadBMPInfoHeader(FILE *fp, BITMAPINFOHEADER *header);
unsigned short ReadLE2(FILE *fp);
unsigned int ReadLE4(FILE *fp);
unsigned int ReadLE8(FILE *fp);
unsigned int ReadLE36(FILE *fp);

/* function.c file */
void mainMenu(int cflag, int uflag, int nflag, int bflag);
FILE* readImageHeader(int imgCount, FILE* fp, BITMAPFILEHEADER *bmpFileHeader, BITMAPINFOHEADER *bmpInfoHeader);
unsigned char* readImageData(FILE *fp, BITMAPFILEHEADER *bmpFileHeader, BITMAPINFOHEADER *bmpInfoHeader);
unsigned char* createBuffer(int width, int height, int bitPerPixel);
unsigned char* binaryImageData(int uflag, unsigned char* data, BITMAPFILEHEADER *bmpFileHeader,BITMAPINFOHEADER *bmpInfoHeader);
void writeBinaryImage(unsigned char* binaryData, int imgCount, BITMAPFILEHEADER *bmpFileHeader, BITMAPINFOHEADER *bmpInfoHeader,unsigned char *data);

#endif