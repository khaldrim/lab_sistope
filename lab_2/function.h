#include "struct.h"

#ifndef _FUNCIONES_H_
#define _FUNCIONES_H_

/* bmp.c file */
BITMAPFILEHEADER *ReadBMPFileHeader(FILE *fp, BITMAPFILEHEADER  *header);
BITMAPINFOHEADER *ReadBMPInfoHeader(FILE *fp, BITMAPINFOHEADER *header);
unsigned short ReadLE2(FILE *fp);
unsigned int ReadLE4(FILE *fp);
unsigned int ReadLE8(FILE *fp);

/* function.c file */
void mainMenu(int cflag, int uflag, int nflag, int bflag);
FILE* readImageHeader(int imgCount, FILE* fp, BITMAPFILEHEADER *bmpFileHeader, BITMAPINFOHEADER *bmpInfoHeader);
void readImageData(FILE *fp, BITMAPFILEHEADER *bmpFileHeader, BITMAPINFOHEADER *bmpInfoHeader, DATA *data);
void createBuffer(int width, int height, int bitPerPixel, DATA *data);
unsigned int* binaryImageData(int uflag, unsigned int* grayData,BITMAPINFOHEADER *bmpInfoHeader);
unsigned int* scaleGrayData(unsigned char** data, BITMAPINFOHEADER *bmpInfoHeader);
void writeBinaryImage(unsigned int* binaryData, int imgCount, BITMAPFILEHEADER *bmpFileHeader, BITMAPINFOHEADER *bmpInfoHeader);
void freeData(unsigned char** data, int width, int height, int bitPerPixel);
int isNearlyBlack(unsigned int *binaryData, int nflag, int width, int height);
void printResult(int* imgPrintResult, int cflag);
#endif