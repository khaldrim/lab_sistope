#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "readImage.h"

/*
 * Descripcion: Funcion que permite leer la cabezera de archivo de la imagen bmp, primero se 
 *              inicializan las variales, luego mediante 'fread' se leen los bytes del archivo
 *              para luego asignar los resultados a la estructura 'BITMAPFILEHEADER'.
 *              Durante la lectura, se apoya en la funcion 'ReadLE4' que permite pasar los bits
 *              de 'Big-Endian' a 'Litle-Endian' dependiendo del tamaño de bytes leidos.
 * 
 * Entrada:     Puntero al archivo 'fp', Puntero a la estructura 'BITMAPFILEHEADER'
 * Salida:      Puntero hacia la estructura 'BMPFILEHEADER'
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
 * Descripcion: Funcion que permite leer la cabezera de informacion de la imagen bmp, primero se 
 *              inicializan las variales, luego mediante 'fread' se leen los bytes del archivo
 *              para luego asignar los resultados a la estructura 'BITMAPINFOHEADER'.
 *              Durante la lectura, se apoya en la funcion 'ReadLE4', 'ReadLE2' y 'ReadLE8' que 
 *              permite pasar los bits de 'Big-Endian' a 'Litle-Endian' dependiendo del tamaño de bytes leidos.
 * 
 * Entrada:     Puntero al archivo 'fp', Puntero a la estructura 'BITMAPINFOHEADER'
 * Salida:      Puntero hacia la estructura 'BMPINFOHEADER'
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
    width = (unsigned long long) ReadLE4(fp);
    height = (unsigned long long) ReadLE4(fp);
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

    // if(headersize == 40)
    // {
    //     width = (unsigned int) ReadLE4(fp);
    //     height = (unsigned int) ReadLE4(fp);
    //     planes = (unsigned short) ReadLE2(fp);
    //     bitcount = (unsigned short) ReadLE2(fp);
    //     compression = (unsigned int) ReadLE4(fp);
    //     sizeImage = (unsigned int) ReadLE4(fp);
    //     xPelsPerMeter = (unsigned long long) ReadLE8(fp);
    //     yPelsperMeter = (unsigned long long) ReadLE8(fp);
    //     used = (unsigned int) ReadLE4(fp);
    //     important = (unsigned int) ReadLE4(fp);

    //     header->size         = headersize;
    //     header->width        = width;
    //     header->height       = height;
    //     header->planes       = planes;
    //     header->bitPerPixel  = bitcount;
    //     header->compression  = compression;
    //     header->sizeImage = sizeImage;
    //     header->xPelsPerMeter = xPelsPerMeter;
    //     header->yPelsperMeter = yPelsperMeter;
    //     header->used = used;
    //     header->important = important;

    // }
    // else
    // {
    //     width = (unsigned int) ReadLE4(fp);
    //     height = (unsigned int) ReadLE4(fp);
    //     planes = (unsigned short) ReadLE2(fp);
    //     bitcount = (unsigned short) ReadLE2(fp);
    //     compression = (unsigned int) ReadLE4(fp);
    //     sizeImage = (unsigned int) ReadLE4(fp);
    //     xPelsPerMeter = (unsigned long long) ReadLE8(fp);
    //     yPelsperMeter = (unsigned long long) ReadLE8(fp);
    //     used = (unsigned int) ReadLE4(fp);
    //     important = (unsigned int) ReadLE4(fp);
    //     redMask = (unsigned int) ReadLE4(fp);
    //     greenMask = (unsigned int) ReadLE4(fp);
    //     blueMask = (unsigned int) ReadLE4(fp);
    //     alphaMask = (unsigned int) ReadLE4(fp);
    //     csType = (unsigned int) ReadLE4(fp);
    //     xRed = (unsigned int) ReadLE4(fp);
    //     yRed = (unsigned int) ReadLE4(fp);
    //     zRed = (unsigned int) ReadLE4(fp);
    //     xGreen = (unsigned int) ReadLE4(fp);
    //     yGreen = (unsigned int) ReadLE4(fp);
    //     zGreen = (unsigned int) ReadLE4(fp);
    //     xBlue = (unsigned int) ReadLE4(fp);
    //     yBlue = (unsigned int) ReadLE4(fp);
    //     zBlue = (unsigned int) ReadLE4(fp);
    //     gammaRed = (unsigned int) ReadLE4(fp);
    //     gammaGreen = (unsigned int) ReadLE4(fp);
    //     gammaBlue = (unsigned int) ReadLE4(fp);
    //     intent = (unsigned int) ReadLE4(fp);
    //     profileData = (unsigned int) ReadLE4(fp);
    //     profileSize = (unsigned int) ReadLE4(fp);
    //     reserved = (unsigned int) ReadLE4(fp);

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

        printf("WIDTH: %llu | HEIGHT: %llu\n", header->width, header->height);
    return header;
}

/*
 * Descripcion: Funcion que permite mover bits desde 'Big-Endian' a 'Litle-Endian',
 *              de tamaño 2 bytes.
 * 
 * Entrada:     Puntero al archivo 'fp'
 * Salida:      Resultado en 'unsigned short'
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
 * Descripcion: Funcion que permite mover bits desde 'Big-Endian' a 'Litle-Endian',
 *              de tamaño 4 bytes.
 * 
 * Entrada:     Puntero al archivo 'fp'
 * Salida:      Resultado en 'unsigned int'
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
 * Descripcion: Funcion que permite mover bits desde 'Big-Endian' a 'Litle-Endian',
 *              de tamaño 8 bytes.
 * 
 * Entrada:     Puntero al archivo 'fp'
 * Salida:      Resultado en 'unsigned int'
 */
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