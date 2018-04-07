#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "encabezados.h"

/*
 * Input      : Puntero hacia el archivo bmp que se leera.
 * Output     : Puntero hacia la estructura BMPFILEHEADER que almacena la informacion de cabezera.
 * Description: Inicializa las variables 
 */
BMPFILEHEADER *ReadBMPFileHeader(FILE *fp, BMPFILEHEADER  *header)
{
    char           fileType[3] = {'\0', '\0', '\0'};
    unsigned int   fileSize;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned long  offset;

    /* File type (2 bytes) */
    fread(&fileType, 1, 2, fp);

    /* File size (4 bytes) */
    fileSize = (unsigned int) ReadLE4(fp);

    /* Reserved 1 (2 bytes) */
    fread(&reserved1, 2, 1, fp);

    /* Reserved 2 (2 bytes) */
    fread(&reserved2, 2, 1, fp);

    /* Offset (4 bytes) */
    offset = (unsigned long) ReadLE4(fp);

    strcpy(header->fileType, fileType);
    header->fileSize  = fileSize;
    header->reserved1 = reserved1;
    header->reserved2 = reserved2;
    header->offBits   = offset;

    return header;
}

/*
 * Input      : 
 * Output     : 
 * Description: 
 */
int SizeOfInformationHeader(FILE *fp)
{
    int headersize;
    unsigned char buf[4];
    int i;

    fread(buf, 1, 4, fp);
    for (i = 3; i >= 0; i--) {
        headersize = (headersize << 8) | (int) buf[i];
    }

    fseek(fp, 14, SEEK_SET);

    return headersize;
}

/*
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

/*
 * Input      : 
 * Output     : 
 * Description: 
 */
BMPINFOOSHEADER *ReadBMPOSInfoHeader(FILE *fp, BMPINFOOSHEADER *header)
{
    unsigned int   headersize;
    int            width;
    int            height;
    unsigned short planes;
    unsigned short bitcount;

    /* Header size (4 bytes) */
    headersize = (unsigned int) ReadLE4(fp);

    /* Width (2 bytes) */
    width = (int) ReadLE2(fp);

    /* Height (2 bytes) */
    height = (int) ReadLE2(fp);

    /* Planes (2 bytes) */
    planes = (unsigned short) ReadLE2(fp);

    /* Bit Count (2 bytes) */
    bitcount = (unsigned short) ReadLE2(fp);

    header->osSize         = headersize;
    header->osWidth        = width;
    header->osHeight       = height;
    header->osColorPlanes  = planes;
    header->osBitsPerPixel = bitcount;

    return header;
}

/*
 * Input      : 
 * Output     : 
 * Description: 
 */
BMPINFOWINHEADER *ReadBMPWinInfoHeader(FILE *fp, BMPINFOWINHEADER *header)
{
    unsigned int     headersize;
    int              width;
    int              height;
    unsigned short   planes;
    unsigned short   bitcount;
    unsigned int     compression;
    unsigned int     size_image;
    int              x_pix_per_meter;
    int              y_pix_per_meter;
    unsigned int     clr_palette;
    unsigned int     clr_used;

    /* Header size (4 bytes) */
    headersize = (unsigned int) ReadLE4(fp);

    /* Width (4 bytes) */
    width = (int) ReadLE4(fp);

    /* Height (4 bytes) */
    height = (int) ReadLE4(fp);

    /* Planes (2 bytes) */
    planes = (unsigned short) ReadLE2(fp);

    /* Bit Count (2 bytes) */
    bitcount = (unsigned short) ReadLE2(fp);

    /* Compression (4 bytes) */
    compression = (unsigned int) ReadLE4(fp);

    /* Size image (4 bytes) */
    size_image = (unsigned int) ReadLE4(fp);

    /* X pix per meter (4 bytes) */
    x_pix_per_meter = (int) ReadLE4(fp);

    /* Y pix per meter (4 bytes) */
    y_pix_per_meter = (int) ReadLE4(fp);

    /* Color used (4 bytes) */
    clr_palette = (unsigned int) ReadLE4(fp);

    /* Color important (4 bytes) */
    clr_used = (unsigned int) ReadLE4(fp);

    header->winSize         = headersize;
    header->winWidth        = width;
    header->winHeight       = height;
    header->winColorPlanes  = planes;
    header->winBitsPerPixel = bitcount;
    header->winCompression  = compression;
    header->winImgSize      = size_image;
    header->winXPixPerMeter = x_pix_per_meter;
    header->winYPixPerMeter = y_pix_per_meter;
    header->winColorPalette = clr_palette;
    header->winColorUsed    = clr_used;

    return header;
}

/*
 * Input      : 
 * Output     : 
 * Description: 
 */
FILE* readImageHeader(int imgCount, FILE *fp,BMPFILEHEADER *bmpfh, BMPINFOOSHEADER *bmpOSIH, BMPINFOWINHEADER *bmpWinIH)
{
    char fileNumber[5];
    char fileName[30] = "imagenes/imagen_";

    sprintf(fileNumber, "%d", imgCount);
    strcat(fileName, fileNumber);
    strcat(fileName, ".bmp");

    /* Formato nombre de archivo .bmp: imagenes/imagen_X.bmp */

    if((fp = fopen(fileName,"rb")) == NULL)
    {
        printf("No se logro abrir el archivo: %s.\n", fileName);
        abort();
    }

    bmpfh = ReadBMPFileHeader(fp, bmpfh);
    if(strcmp(bmpfh->fileType, "BM") != 0)
    {
        printf("El archivo no es un Bitmap.\n");
        abort();
    }

    bmpfh->headersize = SizeOfInformationHeader(fp);
    if(bmpfh->headersize == 12)
    {
        bmpOSIH = ReadBMPOSInfoHeader(fp, bmpOSIH);
    }
    else if(bmpfh->headersize == 40 || bmpfh->headersize == 124)
    {
        bmpWinIH = ReadBMPWinInfoHeader(fp, bmpWinIH);
    }
    else
    {
        printf("%i Bitmap no soportado.\n", bmpfh->headersize);
        exit(1);
    }


    printf("\n\nFile type          = %s\n", bmpfh->fileType);
    printf("File size          = %d bytes\n", bmpfh->fileSize);
    printf("Data offset        = %ld bytes\n", bmpfh->offBits);
    if (bmpfh->headersize == 12) 
    {
        printf("Info header size   = %d bytes\n", bmpOSIH->osSize);
        printf("Width              = %d pixels\n", bmpOSIH->osWidth);
        printf("Height             = %d pixels\n", bmpOSIH->osHeight);
        printf("Planes             = %d\n", bmpOSIH->osColorPlanes);
        printf("Bits per Pixel     = %d bits/pixel\n", bmpOSIH->osBitsPerPixel);
    } 
    
    if (bmpfh->headersize == 40 || bmpfh->headersize == 124) 
    {
        printf("Info header size   = %d bytes\n", bmpWinIH->winSize);
        printf("Width              = %ld pixels\n", bmpWinIH->winWidth);
        printf("Height             = %ld pixels\n", bmpWinIH->winHeight);
        printf("Color Planes       = %d\n", bmpWinIH->winColorPlanes);
        printf("Bits per Pixel     = %d bits/pixel\n", bmpWinIH->winBitsPerPixel);
        printf("Compression        = %d\n", bmpWinIH->winCompression);
        printf("Size image         = %d bytes\n", bmpWinIH->winImgSize);
        printf("X pixels per meter = %ld\n", bmpWinIH->winXPixPerMeter);
        printf("Y pixels per meter = %ld\n", bmpWinIH->winYPixPerMeter);
        printf("Color Palette      = %ld colors\n", bmpWinIH->winColorPalette);
        printf("Color Used         = %ld colors\n", bmpWinIH->winColorUsed);
    }

    return fp;
}