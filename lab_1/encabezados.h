#include "struct.h"

#ifndef _FUNCIONES_H_
#define _FUNCIONES_H_

BMPFILEHEADER *ReadBMPFileHeader(FILE *fp, BMPFILEHEADER  *header);
int SizeOfInformationHeader(FILE *fp);
BMPINFOOSHEADER *ReadBMPOSInfoHeader(FILE *fp, BMPINFOOSHEADER *header);
BMPINFOWINHEADER *ReadBMPWinInfoHeader(FILE *fp, BMPINFOWINHEADER *header);
FILE* readImageHeader(int img_num, FILE* fp,BMPFILEHEADER *bmpfh, BMPINFOOSHEADER *bmpOsIF, BMPINFOWINHEADER *bmpWinIH);
void mainMenu(int cflag, int uflag, int nflag, int bflag);
char* readPixelData(FILE *fp, unsigned char *data,int width, int height, int offset);
int* binaryData(int uflag, unsigned char **data, BMPINFOWINHEADER *bmpIH);
unsigned short ReadLE2(FILE *fp);
unsigned int ReadLE4(FILE *fp);
void printData(unsigned char *data, int imgSize);
//void writeFile(int imgCount,int *binaryData, BMPINFOWINHEADER *bmpIH, BMPFILEHEADER *bmpFH);
void writeFile(int imgCount,int *binaryData, BMPINFOWINHEADER *bmpIH, BMPFILEHEADER *bmpFH);

unsigned char** createBuffer(int width, int height);
unsigned char** readImageData(FILE *fp, BMPINFOWINHEADER *bmpIH, BMPFILEHEADER *bmpFH, RGB *palette, RGB *pixel);

#endif