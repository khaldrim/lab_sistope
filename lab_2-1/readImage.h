#include "struct.h"

#ifndef _READIMAGE_H_
#define _READIMAGE_H_

/* READIMAGE */
DATA* createBuffer(int width, int height, int bitPerPixel, DATA *data);

/* BMP */
BITMAPFILEHEADER *ReadBMPFileHeader(FILE *fp, BITMAPFILEHEADER  *header);
BITMAPINFOHEADER *ReadBMPInfoHeader(FILE *fp, BITMAPINFOHEADER *header);
unsigned short ReadLE2(FILE *fp);
unsigned int ReadLE4(FILE *fp);
unsigned int ReadLE8(FILE *fp);

#endif