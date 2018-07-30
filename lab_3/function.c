#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include "struct.h"
#include "function.h"
#include "pthread_barrier.h"

//Recursos compartidos
int lock_read           = 0;
int lock_gray           = 0;
int lock_gray_loop      = 0;
int lock_bin            = 0;
int lock_black          = 0;
int lock_black_decision = 0;
int lock_check          = 0;

//escalar a grises
int matrix_counter;
int gray_counter_row;
int gray_counter_col;

//binarizar
int bin_counter        = -1;


//nearly black
int row_black_start = 0;
int row_black_end   = 0;
int totalBlack = 0;
int isBlack = 0;

//datos
int totalData;
int totalSize;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_writeNearlyBlack = PTHREAD_MUTEX_INITIALIZER;
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

        gray_counter_row = bmpInfoHeader->height - 1;
        
        initializeData(data, totalSize);
    }
    pthread_mutex_unlock(&lock);
    pthread_barrier_wait(&barrier);

    //Preparando escala de grises
    pthread_mutex_lock(&lock);
    if(lock_gray != -1){
        lock_gray = 1;
        gray_counter_row = bmpInfoHeader->height - 1;
        gray_counter_col = -1;
        matrix_counter = 0;
    }
    pthread_mutex_unlock(&lock);

    //Inicio de la escala a grises
    grayData(data, bmpInfoHeader->height, bmpInfoHeader->width);
    pthread_barrier_wait(&barrier);
    
    //Inicio de binarizar la imagen
    binaryData(data,inputData,bmpInfoHeader->width,bmpInfoHeader->height);
    pthread_barrier_wait(&barrier);

    //Inicio nearlyBlack;
    isNearlyBlack(data, inputData,bmpInfoHeader->width);

    // printf("Fin de threadMain\n");
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

    if(inputData->bflag == 1)
    {
        printf("| Imagen           | NearlyBlack          |\n");
        printf("|-----------------------------------------|\n");
    }

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
        writeBinaryImage(data,inputData,bmpFileHeader,bmpInfoHeader);
        resetGlobalData();
    }

    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&lock_writeNearlyBlack);
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
    // pixelArray = colSize * bmpInfoHeader->height;

    data = createBuffer(bmpInfoHeader->width, bmpInfoHeader->height);

    if(data != NULL)
    {
        int i, j;
        pixel = (RGB*)malloc(sizeof(RGB));

        fseek(fp, bmpFileHeader->offbits, SEEK_SET);

        for(i = bmpInfoHeader->height-1; i >= 0; i--)
        {
            for(j=0; j<colSize ; j+=4)
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

void grayData(DATA *data, int height, int width)
{
    int row = 0, col = 0, counter = 0;
    int sizeCol = width*4;
    unsigned char red, green, blue;
    double scale;

    while(lock_gray_loop != 1){
        
        pthread_mutex_lock(&lock);
        if(gray_counter_row >= 0){
            gray_counter_col += 4;
            if(gray_counter_col > (width*4)){
                gray_counter_col = 3;
                gray_counter_row--;
            }

            if(gray_counter_row >= 0)
                matrix_counter++; 
        
        } else {
            lock_gray_loop = 1;
        }

        row = gray_counter_row;
        col = gray_counter_col;
        counter = matrix_counter;
        pthread_mutex_unlock(&lock);

        if(lock_gray_loop != 1 && row >= 0 && col <= (width*4))
        {
            red   = data->pixelData[row][col-1];
            green = data->pixelData[row][col-2];
            blue  = data->pixelData[row][col-3];
                
            scale = (int)red*0.3 + (int)green*0.59 + (int)blue*0.11;
            data->grayData[counter] = scale;
        }
    }
}

void binaryData(DATA *data, INPUTDATA* inputData, int width, int height)
{
    int counter = 0, valor = 0;

    while(lock_bin != 1)
    {
        pthread_mutex_lock(&lock);
        bin_counter++;
        counter = bin_counter;
        pthread_mutex_unlock(&lock);

        if(counter < totalSize && lock_bin != 1)
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
        }
        else 
        {
            pthread_mutex_lock(&lock);
            lock_bin = 1;
            pthread_mutex_unlock(&lock);
        }
    }
}

void isNearlyBlack(DATA *data, INPUTDATA* inputData, int width)
{
    int i = 0, start_row = 0, end_row = 0, value = 0;
    float final_value = 0;

    while(lock_black != 1)
    {
        pthread_mutex_lock(&lock);
        if(row_black_end < totalSize)
        {
            row_black_end += width - 1;

            start_row = row_black_start;
            end_row   = row_black_end;

            row_black_start = row_black_end;
        }
        else
            lock_black = 1;
        pthread_mutex_unlock(&lock);

        if(end_row < totalSize)
        {
            for(i = start_row; i < end_row; i++)
            {
                if(data->binaryData[i] == 0)
                {
                    value++;
                }
            }
        }
        
        pthread_mutex_lock(&lock_writeNearlyBlack);
        if(end_row < totalSize)
        {
            totalBlack += value;
            value = 0;
        }   
        pthread_mutex_unlock(&lock_writeNearlyBlack);
    }

    pthread_mutex_lock(&lock);
    if(lock_black_decision != 1)
    {
        lock_black_decision = 1;
        final_value = ((float)totalBlack/(float)totalSize) * 100;
        
        if(inputData->bflag == 1)
        {
            if(final_value > inputData->nflag)
            {
                isBlack = 1;
                printf("| imagen_%i         | Yes                  |\n", inputData->imgCount);
            } 
            else
            {
                printf("| imagen_%i         | No                  |\n", inputData->imgCount);
            }
        }
        
    }
    pthread_mutex_unlock(&lock);
}

void writeBinaryImage(DATA* data, INPUTDATA* inputData, BITMAPFILEHEADER* bmpFileHeader, BITMAPINFOHEADER* bmpInfoHeader)
{
    FILE *fp = NULL;

    char fileNumber[5];
    char fileName[50] = "imagenes/resultados/resultado_imagen_";

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

unsigned char** createBuffer(int width, int height)
{
    unsigned char** data = NULL;
    int colSize, i;

    colSize = width * 4;

    data = (unsigned char**)malloc(sizeof(unsigned char*) * height);

    if(data != NULL)
    {
        for(i=0; i < height; i++)
        {
            data[i] = (unsigned char*)malloc(sizeof(unsigned char) * colSize);
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

void resetGlobalData()
{    
    lock_read           = 0;
    lock_gray           = 0;
    lock_gray_loop      = 0;
    lock_bin            = 0;
    lock_black          = 0;
    lock_black_decision = 0;
    lock_check          = 0;
    bin_counter         = -1;
    row_black_start     = 0;
    row_black_end       = 0;
    totalBlack          = 0;
    isBlack             = 0;
}