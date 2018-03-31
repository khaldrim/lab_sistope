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
char* scaleGreyData(int uflag, unsigned char *data, unsigned char *binary_data, int width, int height);
//void writeBinaryImageWin(unsigned char *binary_data, BMPFILEHEADER *bmpfh, BMPINFOWINHEADER *bmpWinIH);

unsigned short ReadLE2(FILE *fp);
unsigned int ReadLE4(FILE *fp);

#endif