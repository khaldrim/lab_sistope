#include <stdio.h>
#include "struct.h"

#ifndef _BMP_H_
#define _BMP_H_

/* BMP Funcions */
BITMAPFILEHEADER *ReadBMPFileHeader(FILE *fp, BITMAPFILEHEADER  *header);
BITMAPINFOHEADER *ReadBMPInfoHeader(FILE *fp, BITMAPINFOHEADER *header);
unsigned short ReadLE2(FILE *fp);
unsigned int ReadLE4(FILE *fp);
unsigned int ReadLE8(FILE *fp);

#endif