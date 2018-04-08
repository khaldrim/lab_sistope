#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "function.h"

/*
 * Input      : Puntero hacia el archivo bmp que se leera.
 * Output     : Puntero hacia la estructura BMPFILEHEADER que almacena la informacion de cabezera.
 * Description: Inicializa las variables 
 */
BITMAPFILEHEADER *ReadBMPFileHeader(FILE *fp, BITMAPFILEHEADER  *header)
{
    char           fileType[3] = {'\0', '\0', '\0'};
    unsigned int   fileSize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned long  offset;

    fread(&fileType, 1, 2, fp);
    fileSize = (unsigned int) ReadLE4(fp);
    fread(&reserved1, 2, 1, fp);
    fread(&reserved2, 2, 1, fp);
    offset = (unsigned long) ReadLE4(fp);

    strcpy(header->type, fileType);
    header->size  = fileSize;
    header->reserved1 = reserved1;
    header->reserved2 = reserved2;
    header->offbits   = offset;

    return header;
}

/*
 * Input      : 
 * Output     : 
 * Description: 
 */
BITMAPINFOHEADER *ReadBMPInfoHeader(FILE *fp, BITMAPINFOHEADER *header)
{
    unsigned int  headersize;
    unsigned long long  width;
    unsigned long long  height;
    unsigned short planes;
    unsigned short bitcount;;
    unsigned int  compression;
    unsigned int  sizeImage;
    unsigned long long  xPelsPerMeter;
    unsigned long long  yPelsperMeter;
    unsigned int  used;
    unsigned int  important;
    unsigned int  redMask;
    unsigned int  greenMask;
    unsigned int  blueMask;
    unsigned int  alphaMask;
    unsigned int  csType;
    unsigned int  xRed;
    unsigned int  yRed;
    unsigned int  zRed;
    unsigned int  xGreen;
    unsigned int  yGreen;
    unsigned int  zGreen;
    unsigned int  xBlue;
    unsigned int  yBlue;
    unsigned int  zBlue;
    unsigned int  gammaRed;
    unsigned int  gammaGreen;
    unsigned int  gammaBlue;
    unsigned int  intent;
    unsigned int  profileData;
    unsigned int  profileSize;
    unsigned int  reserved; 

    headersize = (unsigned int) ReadLE4 (fp);
    width = (unsigned int) ReadLE4(fp);
    height = (unsigned int) ReadLE4(fp);
    planes = (unsigned short) ReadLE2(fp);
    bitcount = (unsigned short) ReadLE2(fp);
    compression = (unsigned int) ReadLE4(fp);
    sizeImage = (unsigned int) ReadLE4(fp);
    xPelsPerMeter = (unsigned long long) ReadLE8(fp);
    yPelsperMeter = (unsigned long long) ReadLE8(fp);
    used = (unsigned int) ReadLE4(fp);
    important = (unsigned int) ReadLE4(fp);
    redMask = (unsigned int) ReadLE4(fp);
    greenMask = (unsigned int) ReadLE4(fp);
    blueMask = (unsigned int) ReadLE4(fp);
    alphaMask = (unsigned int) ReadLE4(fp);
    csType = (unsigned int) ReadLE4(fp);
    xRed = (unsigned int) ReadLE4(fp);
    yRed = (unsigned int) ReadLE4(fp);
    zRed = (unsigned int) ReadLE4(fp);
    xGreen = (unsigned int) ReadLE4(fp);
    yGreen = (unsigned int) ReadLE4(fp);
    zGreen = (unsigned int) ReadLE4(fp);
    xBlue = (unsigned int) ReadLE4(fp);
    yBlue = (unsigned int) ReadLE4(fp);
    zBlue = (unsigned int) ReadLE4(fp);
    gammaRed = (unsigned int) ReadLE4(fp);
    gammaGreen = (unsigned int) ReadLE4(fp);
    gammaBlue = (unsigned int) ReadLE4(fp);
    intent = (unsigned int) ReadLE4(fp);
    profileData = (unsigned int) ReadLE4(fp);
    profileSize = (unsigned int) ReadLE4(fp);
    reserved = (unsigned int) ReadLE4(fp);
    
    header->size         = headersize;
    header->width        = width;
    header->height       = height;
    header->planes       = planes;
    header->bitPerPixel  = bitcount;
    header->compression  = compression;
    header->sizeImage = sizeImage;
    header->xPelsPerMeter = xPelsPerMeter;
    header->yPelsperMeter = yPelsperMeter;
    header->used = used;
    header->important = important;
    header->redMask = redMask;
    header->greenMask = greenMask;
    header->blueMask = blueMask;
    header->alphaMask = alphaMask;
    header->csType = csType;
    header->ciexyzXRed = xRed;
    header->ciexyzYRed = yRed;
    header->ciexyzZRed = zRed;
    header->ciexyzXGreen = xGreen;
    header->ciexyzYGreen = yGreen;
    header->ciexyzZGreen = zGreen;
    header->ciexyzXBlue = xBlue;
    header->ciexyzYBlue = yBlue;
    header->ciexyzZBlue = zBlue;
    header->gammaRed = gammaRed;
    header->gammaGreen = gammaGreen;
    header->gammaBlue = gammaBlue;
    header->intent = intent;
    header->profileData = profileData;
    header->profileSize = profileSize;
    header->reserved = reserved;

    return header;
}

/*bitsPerPixel
 * Input      : 
 * Output     : 
 * Description: 
 */
unsigned short ReadLE2(FILE *fp)
{
    unsigned char buf[2];
    unsigned short result = 0;
    int i;

    fread(buf, 1, 2, fp);
    for (i = 1; i >= 0; i--) {
        result = (result << 8) | (unsigned short) buf[i];
    }

    return result;
}

/*
 * Input      : 
 * Output     : 
 * Description: 
 */
unsigned int ReadLE4(FILE *fp)
{
    unsigned char buf[4];
    unsigned int result = 0;
    int i;

    fread(buf, 1, 4, fp);
    for (i = 3; i >= 0; i--) {
        result = (result << 8) | (unsigned int) buf[i];
    }

    return result;
}

unsigned int ReadLE8(FILE *fp)
{
    unsigned char buf[8];
    unsigned int result = 0;
    int i;

    fread(buf, 1, 8, fp);
    for (i = 7; i >= 0; i--) {
        result = (result << 8) | (unsigned int) buf[i];
    }

    return result;
}

unsigned int ReadLE36(FILE *fp)
{
    unsigned char buf[36];
    unsigned int result = 0;
    int i;

    fread(buf, 1, 36, fp);
    for (i = 35; i >= 0; i--) {
        result = (result << 8) | (unsigned int) buf[i];
    }

    return result;
}