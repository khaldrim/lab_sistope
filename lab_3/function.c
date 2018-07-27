#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include "struct.h"
#include "function.h"
#include "pthread_barrier.h"

//Recursos compartidos
int lock_read          = 0;
int lock_gray          = 0;
int lock_bin           = 0;

int lock_check         = 0;

//escalar a grises
int matrix_counter     = 0;
int gray_counter_row   = 0;
int gray_counter_col   = 0;

//binarizar
int bin_counter        = -1;

int totalData;
int totalSize;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockGray = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;

DATA *data;
BITMAPFILEHEADER *bmpFileHeader;
BITMAPINFOHEADER *bmpInfoHeader;

void *threadMain(void *input)
{
    INPUTDATA *inputData = (INPUTDATA*)input;

    //Inicio de la lectura
    pthread_mutex_lock(&lock);
    if(lock_read == 0) {
        lock_read = 1;
        data = (DATA*)malloc(sizeof(DATA));
        bmpFileHeader = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
        bmpInfoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
        data->pixelData  = readBMPImage(inputData->imgCount, bmpFileHeader, bmpInfoHeader);
        
        totalSize = bmpInfoHeader->width * bmpInfoHeader->height;
        totalData = totalSize * 4;

        printf("bitmap width: %llu | bitmap height: %llu\n", bmpInfoHeader->width, bmpInfoHeader->height);
        printf("totalSize: %i | totalData: %i\n", totalSize, totalData);
        
        initializeData(data, totalSize);
    }

    pthread_mutex_unlock(&lock);
    pthread_barrier_wait(&barrier);

    //Inicio de la escala a grises
    grayData(data, bmpInfoHeader->height, bmpInfoHeader->width);
    pthread_barrier_wait(&barrier);

    pthread_mutex_lock(&lock);
    if(lock_bin == 0)
    {
        lock_bin = 1;
        int valor = 0;
        valor = checkPixelData(data,bmpInfoHeader->width,bmpInfoHeader->height);
        if(valor == -1){
            printf("error en escalar\n");
        }
        else{
            printf("tamo tranquilo'\n");
        }
    }
    pthread_mutex_unlock(&lock);
    pthread_barrier_wait(&barrier);
    //Inicio de binarizar la imagen
    binaryData(data,inputData,bmpInfoHeader->width,bmpInfoHeader->height);
    pthread_barrier_wait(&barrier);

    pthread_mutex_lock(&lock);
    if(lock_check == 0)
    {
        lock_check = 1;
        int valor = 0;
        valor = checkBinData(data,bmpInfoHeader->width,bmpInfoHeader->height);
        if(valor == -1){
            printf("error en binarizar\n");
        }
        else{
            printf("tamo tranquilo'\n");
        }
    }
    pthread_mutex_unlock(&lock);


    printf("Fin de threadMain\n");
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
    pthread_barrier_init(&barrier, NULL, hflag);

    while(imgCount < cflag)
    {
        int k;
        inputData->imgCount = imgCount;

        for(k = 0; k < hflag; k++) {
            if(pthread_create(&threadGroup[k], NULL, threadMain, inputData)) {
                fprintf(stderr, "Error creating thread\n");
                return 1;
            }
        }

        for(k = 0; k < hflag; k++) {
            if(pthread_join(threadGroup[k], NULL)) {
                fprintf(stderr, "Error joining thread\n");
                return 2;
            }
        }

        imgCount++;
        lock_read = 0;
        lock_gray = 0;
        lock_bin  = 0;
        matrix_counter = 0;
        gray_counter_col = 0;
        gray_counter_row = 0;

        printf("antes de imprimir\n");
        writeBinaryImage(data,inputData,bmpFileHeader,bmpInfoHeader);
        printf("despues de imprimir\n");
    }

    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&lockGray);
    pthread_barrier_destroy(&barrier);

    return 0;
}

 unsigned char** readBMPImage(int imgCount, BITMAPFILEHEADER* bmpFileHeader, BITMAPINFOHEADER* bmpInfoHeader)
 {
    FILE *fp;
    unsigned char **data = NULL;
    int colSize, pixelArray;
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

    colSize = bmpInfoHeader->width * 4;
    pixelArray = colSize * bmpInfoHeader->height;

    data = createBuffer(bmpInfoHeader->width, bmpInfoHeader->height, bmpInfoHeader->bitPerPixel);

    if(data != NULL)
    {
        int i, j;
        pixel = (RGB*)malloc(sizeof(RGB));

        fseek(fp, bmpFileHeader->offbits, SEEK_SET);

        for(i=bmpInfoHeader->width-1;i>0;i--)
        {
            for(j=0; j< colSize; j+=4)
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
    int colSize, i;

    colSize = width * 4;
    data = (unsigned char**)malloc(sizeof(unsigned char*) * colSize);

    if(data != NULL)
    {
        for(i=0; i< colSize; i++)
        {
            data[i] = (unsigned char*)malloc(sizeof(unsigned char) * height);
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

DATA* initializeData(DATA *data, int totalSize)
{
    int k = 0;
    data->grayData   = (unsigned int*)malloc(sizeof(unsigned int) * totalSize);
    data->binaryData = (unsigned int*)malloc(sizeof(unsigned int) * totalSize);

    for(k = 0;k < totalSize; k++)
    {
        data->grayData[k] = -1;
        data->binaryData[k] = -1;
    }

    return data;
}

void grayData(DATA *data, int height, int width)
{
    int row = 0, col = 0, counter = 0;
    unsigned char red, green, blue;
    double scale;

    while(lock_gray != 1)
    {
        pthread_mutex_lock(&lock);
        if(gray_counter_row < width)
        {
            gray_counter_col += 4;
            if(gray_counter_col > (height*4))
            {
                gray_counter_col = 4;
                gray_counter_row++;
            }

            matrix_counter++; 
        }
        pthread_mutex_unlock(&lock);

        row = gray_counter_row;
        col = gray_counter_col;
        counter = matrix_counter;

        if(counter >= totalSize)
            lock_gray = 1;

        if(lock_gray != 1)
        {
            if(row < width && col <= (height*4))
            {
                red   = data->pixelData[row][col-1];
                green = data->pixelData[row][col-2];
                blue  = data->pixelData[row][col-3];
                
                scale = (int)red*0.3 + (int)green*0.59 + (int)blue*0.11;
                data->grayData[counter] = scale;
                
                
            }
        }

        // printf("row: %i | col: %i | counter: %i | matrix_counter: %i | gray_row: %i | gray_col: %i\n", row, col, counter,matrix_counter, gray_counter_row, gray_counter_col);
    }

    // printf("fin escala de grises\n");
}

void binaryData(DATA *data, INPUTDATA* inputData, int width, int height)
{
    int size = 0, counter = 0, valor = 0;

    while(lock_bin != 1)
    {
        pthread_mutex_lock(&lock);
        bin_counter++;
        counter = bin_counter;
        pthread_mutex_unlock(&lock);

        if(counter < totalSize)
        {
            valor = data->grayData[counter];
            if(valor > inputData->uflag)
            {
                data->binaryData[counter] = 1;
            }
            else
            {
                data->binaryData[counter] = 0;
            }

            // printf("counter: %i | valor: %i\n", counter, data->binaryData[counter]);
        }
        else 
        {
            pthread_mutex_lock(&lock);
            lock_bin = 1;
            pthread_mutex_unlock(&lock);
        }
    }
}

int checkPixelData(DATA *data, int width, int height)
{
    int i = 0, valor;
    for(i = 0;i < (height*width);i++)
    {
        valor = data->grayData[i];
        if(valor == -1)
        {
            // printf("error culiao\n");
            return 1;
        }

        // printf("asd: %i %i\n",valor, i);

    }

    return 0;
}

int checkBinData(DATA *data, int width, int height)
{
    int i = 0, valor;
    for(i = 0;i < (height*width);i++)
    {
        valor = data->binaryData[i];
        if(valor == -1)
        {
            // printf("error culiao\n");
            return 1;
        }
    }

    return 0;
}


void writeBinaryImage(DATA* data, INPUTDATA* inputData, BITMAPFILEHEADER* bmpFileHeader, BITMAPINFOHEADER* bmpInfoHeader)
{
    FILE *fp = NULL;

    char fileNumber[5];
    char fileName[35] = "imagenes/resultado_imagen_";

    sprintf(fileNumber, "%d", inputData->imgCount);
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
    while(k < totalSize)
    {
        if(data->binaryData[k] == 1)
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