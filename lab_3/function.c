#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include "struct.h"
#include "function.h"
#include "pthread_barrier.h"

//Recursos compartidos

int lock_read = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t readImage;

DATA *data;
BITMAPFILEHEADER *bmpFileHeader;
BITMAPINFOHEADER *bmpInfoHeader;

void *threadMain(void *input) 
{
    INPUTDATA *inputData = (INPUTDATA*)input;

    pthread_mutex_lock(&lock);
    if(lock_read == 0) {
        data = (DATA*)malloc(sizeof(DATA));
        bmpFileHeader = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
        bmpInfoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));

        data->pixelData = readBMPImage(inputData->imgCount, bmpFileHeader, bmpInfoHeader);
        lock_read++;
        printf("bitmap width: %llu\n", bmpInfoHeader->width);
    }
    pthread_mutex_unlock(&lock);

    //Barrera para esperar la lectura.

    return NULL;
}


int mainMenu(int cflag, int hflag, int uflag, int nflag, int bflag)
{   
    int imgCount = 0;
    INPUTDATA *inputData = (INPUTDATA*) malloc(sizeof(INPUTDATA));

    inputData->cflag = cflag;
    inputData->hflag = hflag;
    inputData->uflag = uflag;
    inputData->nflag = nflag;
    inputData->bflag = bflag;

    pthread_t threadGroup[hflag];
    
    // printf("cflag=%d, hflag=%d,uflag=%d, nflag=%d, bflag=%d \n", inputData->cflag, inputData->hflag,inputData->uflag,inputData->nflag,inputData->bflag);

    while(imgCount < cflag) 
    {
        int k;
        pthread_barrier_init(&readImage, NULL, hflag);
        inputData->imgCount = imgCount;

        for(k = 0; k < hflag; k++) {
            if(pthread_create(&threadGroup[k], NULL, threadMain, inputData)) {
                fprintf(stderr, "Error creating thread\n");
                return 1;
            }
        }

        pthread_mutex_destroy(&lock);
        pthread_barrier_destroy(&readImage);

        for(k = 0; k < hflag; k++) {
            if(pthread_join(threadGroup[k], NULL)) {
                fprintf(stderr, "Error joining thread\n");
                return 2;
            }
        }

        imgCount++;
        lock_read = 0;
    }

    return 0;
}


 unsigned char** readBMPImage(int imgCount, BITMAPFILEHEADER* bmpFileHeader, BITMAPINFOHEADER* bmpInfoHeader)
 {
    FILE *fp;
    unsigned char **data = NULL;
    int rowSize, pixelArray;
    RGB *pixel;
    char fileNumber[5];
    char fileName[30] = "imagenes/imagen_";

    sprintf(fileNumber, "%d", imgCount);
    strcat(fileName, fileNumber);
    strcat(fileName, ".bmp");

    if((fp = fopen(fileName,"rb")) == NULL)
    {
        printf("No se logro abrir el archivo: %s.\n", fileName);
        exit(1);
    }

    bmpFileHeader = ReadBMPFileHeader(fp, bmpFileHeader);
    bmpInfoHeader = ReadBMPInfoHeader(fp, bmpInfoHeader);

    rowSize = (((bmpInfoHeader->bitPerPixel * bmpInfoHeader->width) + 31) / 32) * 4;
    pixelArray = rowSize * bmpInfoHeader->height;

    data = createBuffer(bmpInfoHeader->width, bmpInfoHeader->height, bmpInfoHeader->bitPerPixel);
    
    if(data != NULL)
    {
        int i, j;
        pixel = (RGB*)malloc(sizeof(RGB));
        
        fseek(fp, bmpFileHeader->offbits, SEEK_SET);

        for(i=bmpInfoHeader->height-1;i>0;i--)
        {
            for(j=0;j<rowSize;j+=4)
            {
                fread(pixel, sizeof(RGB), 1, fp);
                data[i][j]   = pixel->blue;
                data[i][j+1] = pixel->green;
                data[i][j+2] = pixel->red;
                data[i][j+3] = pixel->alpha;
            }
        }
        fclose(fp);
        return data;
    }
    else
    {
        printf("No se pudo asignar memoria para leer los datos de la imagen.\n");
        exit(1);
    }

    return NULL;
 }

 unsigned char** createBuffer(int width, int height, int bitPerPixel)
{
    unsigned char** data = NULL;
    int rowSize, pixelArray, i;

    rowSize = (((bitPerPixel * width) + 31) / 32) * 4;
    pixelArray = rowSize * height;
    data = (unsigned char**)malloc(sizeof(unsigned char*) * height);

    if(data != NULL)
    {
        for(i=0; i< height; i++)
        {
            data[i] = (unsigned char*)malloc(sizeof(unsigned char) * rowSize);
            if(data[i] == NULL)
            {
                printf("No existe espacio para asignar memoria a las filas de la matriz.\n");
                exit(1);
            }
        }

        return data;
    }
    else
    {
        printf("No hay espacio para los datos de la imagen.\n");
        exit(1);
    }
}

