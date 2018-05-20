#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "struct.h"
#include "bmp.h"

#define READ 0
#define WRITE 1

/* Cabecera de funciones */
FILE* readImageHeader(int imgCount, FILE* fp, BITMAPFILEHEADER *bmpFileHeader, BITMAPINFOHEADER *bmpInfoHeader);
void writeBinaryImage(unsigned int* binaryData, int imgCount, BITMAPFILEHEADER *bmpFileHeader, BITMAPINFOHEADER *bmpInfoHeader);

/*
 * Descripcion: Recibe los datos del proceso anterior por la entrada estandar, primero los lee y asigna. Luego lee dato por dato binarizado y los asigna
 *              al arreglo local. Luego todos estos datos se envian a la funcion 'writeBinaryImage' quien es el encargado de escribir la imagen resultado.
 * 
 * Entrada: Por argumento nada, por la entrada estandar: cflag, width, height, offset, datos de los pixeles binarizados.
 * 
 * Salida: Ninguna.
*/
int main(int argc, char* argv[])
{
    int cflag, totalSize, i;
    unsigned int offbits;
    unsigned long long width, height;
    unsigned int* binaryData; 

    FILE *fp = NULL;
    BITMAPFILEHEADER *bmpFileHeader = NULL;
    BITMAPINFOHEADER *bmpInfoHeader = NULL;

    bmpFileHeader = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
    bmpInfoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));

    read(STDIN_FILENO, &cflag, sizeof(int));
    read(STDIN_FILENO, &width, sizeof(unsigned long long));
    read(STDIN_FILENO, &height, sizeof(unsigned long long));
    read(STDIN_FILENO, &offbits, sizeof(unsigned int));

    fp = readImageHeader(cflag, fp, bmpFileHeader, bmpInfoHeader);
    fclose(fp);
    totalSize = width * height;
    
    binaryData = (unsigned int*)malloc(sizeof(unsigned int) * totalSize);

    for(i = 0; i < totalSize; i++)
    {
        read(STDIN_FILENO, &binaryData[i], sizeof(unsigned int));
    }

    /* Se escribe el archivo con los datos binarizados. */
    writeBinaryImage(binaryData, cflag, bmpFileHeader, bmpInfoHeader);
    return 0;
}

/*
 * Descripcion: La funcion realiza una concatenacion para lograr el nombre correcto de la imagen, se intenta
 *              abrir la imagen, si no es posible abrir la imagen se detiene el programa. Luego se llama a la
 *              funcion que lee la cabecera del archivo 'ReadBMPFileHeader' y a la funcion que lee la informacion
 *              de cabecera 'ReadBMPInfoHeader', ambas funciones se encuentran en el archivo 'bmp.c'. Se retorna
 *              el puntero de la imagen que se logro abrir.
 * 
 * Entrada: Contador de imagenes 'imgCount', Puntero a un archivo 'fp', Puntero a la estructura BITMAPFILEHEADER 'bmpFileHeader',
 *          Puntero a la estructura BITMAPINFOHEADER 'bmpInfoHeader'.
 * 
 * Salida: Puntero de la imagen que se logro abrir.
 */ 
FILE* readImageHeader(int imgCount, FILE* fp, BITMAPFILEHEADER *bmpFileHeader, BITMAPINFOHEADER *bmpInfoHeader)
{
    char fileNumber[5];
    char fileName[30] = "imagenes/imagen_";

    sprintf(fileNumber, "%d", imgCount);
    strcat(fileName, fileNumber);
    strcat(fileName, ".bmp");

    /* 
     *   Formato nombre de archivo .bmp: imagenes/imagen_X.bmp 
     *   Por ahora solo leemos imagenes de 32 bpp con un headerSize de 124 bytes 
     */

    fp = fopen(fileName,"rb");
    if(fp == NULL)
    {
        printf("No se logro abrir el archivo: %s.\n", fileName);
        exit(EXIT_FAILURE);
    }
    
    bmpFileHeader = ReadBMPFileHeader(fp, bmpFileHeader);
    bmpInfoHeader = ReadBMPInfoHeader(fp, bmpInfoHeader);
    
    return fp;
}

/*
 * Descripcion: Esta funcion permite crear un archivo .bmp con el nombre de 'resultado_imagen_x.bmp', primero abrimos un archivo
 *              con el nombre indicado, para luego proceder a escribir primero los datos que tenemos guardados en la estructura
 *              BITMAPFILEHEADER, luego de escribir estos datos de archivo de cabecera, procedemos a escribir los datos de
 *              informacion de cabecera guardados en la esturctura BITMAPINFOHEADER. Una vez escrito estos datos procedemos a
 *              escribir la imagen pixel por pixel utilizando los datos binarizados obtenidos de la variable 'binaryData'.
 *              Luego cerramos el archivo resultado.
 * 
 * Entrada: Arreglo de enteros 'binaryData', Entero 'imgCount', Puntero a estructura BITMAPFILEHEADER 'bmpFileHeader',
 *          Putero a estructura 'bmpInfoHeader'.
 * 
 * Salida: Vacia. 
 */
void writeBinaryImage(unsigned int* binaryData, int imgCount, BITMAPFILEHEADER *bmpFileHeader, BITMAPINFOHEADER *bmpInfoHeader)
{
    FILE *fp = NULL;

    char fileNumber[5];
    char fileName[50] = "imagenes/resultado_imagen_";

    sprintf(fileNumber, "%d", imgCount);
    strcat(fileName, fileNumber);
    strcat(fileName, ".bmp");

    if((fp=fopen(fileName, "wb")) == NULL)
    {
        printf("No se logro abrir el archivo: %s.\n", fileName);
        exit(1);   
    }

    /* FILE HEADER */
    fwrite(&bmpFileHeader->type, 2, 1, fp);
    fwrite(&bmpFileHeader->size, 4, 1, fp);
    fwrite(&bmpFileHeader->reserved1, 2, 1, fp);
    fwrite(&bmpFileHeader->reserved2, 2, 1, fp);
    fwrite(&bmpFileHeader->offbits, 4, 1, fp);

    /* INFO HEADER */
    fwrite(&bmpInfoHeader->size, 4, 1, fp);
    fwrite(&bmpInfoHeader->width, 4, 1, fp);
    fwrite(&bmpInfoHeader->height, 4, 1, fp);
    fwrite(&bmpInfoHeader->planes, 2, 1, fp);
    fwrite(&bmpInfoHeader->bitPerPixel, 2, 1, fp);
    fwrite(&bmpInfoHeader->compression, 4, 1, fp);
    fwrite(&bmpInfoHeader->sizeImage, 4, 1, fp);
    fwrite(&bmpInfoHeader->xPelsPerMeter, 8, 1, fp);
    fwrite(&bmpInfoHeader->yPelsperMeter, 8, 1, fp);
    fwrite(&bmpInfoHeader->used, 4, 1, fp);
    fwrite(&bmpInfoHeader->important, 4, 1, fp);
    fwrite(&bmpInfoHeader->redMask, 4, 1, fp);
    fwrite(&bmpInfoHeader->greenMask, 4, 1, fp);
    fwrite(&bmpInfoHeader->blueMask, 4, 1, fp);
    fwrite(&bmpInfoHeader->alphaMask, 4, 1, fp);
    fwrite(&bmpInfoHeader->csType, 4, 1, fp);
    fwrite(&bmpInfoHeader->ciexyzXRed, 4, 1, fp);
    fwrite(&bmpInfoHeader->ciexyzYRed, 4, 1, fp);
    fwrite(&bmpInfoHeader->ciexyzZRed, 4, 1, fp);
    fwrite(&bmpInfoHeader->ciexyzXGreen, 4, 1, fp);
    fwrite(&bmpInfoHeader->ciexyzYGreen, 4, 1, fp);
    fwrite(&bmpInfoHeader->ciexyzZGreen, 4, 1, fp);
    fwrite(&bmpInfoHeader->ciexyzXBlue, 4, 1, fp);
    fwrite(&bmpInfoHeader->ciexyzYBlue, 4, 1, fp);
    fwrite(&bmpInfoHeader->ciexyzZBlue, 4, 1, fp);
    fwrite(&bmpInfoHeader->gammaRed, 4, 1, fp);
    fwrite(&bmpInfoHeader->gammaGreen, 4, 1, fp);
    fwrite(&bmpInfoHeader->gammaBlue, 4, 1, fp);
    fwrite(&bmpInfoHeader->intent, 4, 1, fp);
    fwrite(&bmpInfoHeader->profileData, 4, 1, fp);
    fwrite(&bmpInfoHeader->profileSize, 4, 1, fp);
    fwrite(&bmpInfoHeader->reserved, 4, 1, fp);
    

    RGB* pixel = (RGB*)malloc(sizeof(RGB));
    int k=0;
    int totalSize = bmpInfoHeader->height * bmpInfoHeader->width; 
    while(k < totalSize)
    {
        if(binaryData[k] == 1)
        {
            pixel->blue = 255;
            pixel->green = 255;
            pixel->red = 255;
            pixel->alpha = 255;
            fwrite(pixel, sizeof(RGB), 1, fp);
        }
        else
        {
            pixel->blue = 0;
            pixel->green = 0;
            pixel->red = 0;
            pixel->alpha = 255;
            fwrite(pixel, sizeof(RGB), 1, fp);
        }

        k++;
    }
    fclose(fp);
}