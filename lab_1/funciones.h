#include "struct.h"

#ifndef _FUNCIONES_H_
#define _FUNCIONES_H_

BMPFILEHEADER *ReadBMPFileHeader(FILE *fp, BMPFILEHEADER  *header);
int SizeOfInformationHeader(FILE *fp);
BMPINFOOSHEADER *ReadBMPOSInfoHeader(FILE *fp, BMPINFOOSHEADER *header);
BMPINFOWINHEADER *ReadBMPWinInfoHeader(FILE *fp, BMPINFOWINHEADER *header);

FILE* readImageHeader(int img_num, FILE* fp,BMPFILEHEADER *bmpfh, BMPINFOOSHEADER *bmpOsIF, BMPINFOWINHEADER *bmpWinIH);
void mainMenu(int cflag, int uflag, int nflag, int bflag);
void readPixel(FILE *fp);

unsigned short ReadLE2(FILE *fp);
unsigned int ReadLE4(FILE *fp);

#endif