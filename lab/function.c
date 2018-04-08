#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "struct.h"
#include "function.h"

void mainMenu(int cflag, int uflag, int nflag, int bflag)
{
    int cValue, imgCount;

    cValue = cflag;
    imgCount = 1;
    while(cValue > 0)
    {
        FILE *fp = NULL;
        unsigned char **data = NULL;
        unsigned int *binaryData = NULL;
        BITMAPFILEHEADER *bmpFileHeader = NULL;
        BITMAPINFOHEADER *bmpInfoHeader = NULL;

        bmpFileHeader = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
        bmpInfoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));

        fp = readImageHeader(imgCount, fp, bmpFileHeader, bmpInfoHeader);

        printf("chaos\n");
        printf("Ok\n");
        data = readImageData(fp, bmpFileHeader, bmpInfoHeader);
        fclose(fp);

        binaryData = binaryImageData(uflag, data, bmpFileHeader, bmpInfoHeader);
        writeBinaryImage(binaryData, imgCount, bmpFileHeader,bmpInfoHeader);   

        cValue--;
        imgCount++;

        free(bmpFileHeader);
        free(bmpInfoHeader);
    }


    printf("FIN.\n");
}

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

    if((fp = fopen(fileName,"rb")) == NULL)
    {
        printf("No se logro abrir el archivo: %s.\n", fileName);
        abort();
    }
    
    bmpFileHeader = ReadBMPFileHeader(fp, bmpFileHeader);
    bmpInfoHeader = ReadBMPInfoHeader(fp, bmpInfoHeader);

    printf("\n\n # Image Data #\n");
    printf("File type          = %s\n", bmpFileHeader->type);
    printf("File size          = %d bytes\n", bmpFileHeader->size);
    printf("Data offset        = %d bytes\n", bmpFileHeader->offbits);
    printf("Header Size        = %d bytes\n", bmpInfoHeader->size);
    printf("Width              = %lld pixels\n", bmpInfoHeader->width);
    printf("Height             = %lld pixels\n", bmpInfoHeader->height);
    printf("Planes             = %d\n", bmpInfoHeader->planes);
    printf("Bits per Pixel     = %d bits/pixel\n", bmpInfoHeader->bitPerPixel);
    printf("Compression        = %d \n", bmpInfoHeader->compression);
    printf("Size Image         = %d \n", bmpInfoHeader->sizeImage);
    printf("xPelsperMeter      = %llu \n", bmpInfoHeader->xPelsPerMeter);
    printf("yPelsPerMeter      = %llu \n", bmpInfoHeader->yPelsperMeter);
    printf("used               = %d \n", bmpInfoHeader->used);
    printf("important          = %d \n", bmpInfoHeader->important);
    printf("redMask            = %d \n", bmpInfoHeader->redMask);
    printf("greenMask          = %d \n", bmpInfoHeader->greenMask);
    printf("blueMask           = %d \n", bmpInfoHeader->blueMask);
    printf("alphaMask          = %d \n", bmpInfoHeader->alphaMask);
    printf("csType             = %d \n", bmpInfoHeader->csType);
    printf("xRed             = %d \n", bmpInfoHeader->ciexyzXRed);
    printf("yRed             = %d \n", bmpInfoHeader->ciexyzYRed);
    printf("zRed             = %d \n", bmpInfoHeader->ciexyzZRed);
    printf("xGreen             = %d \n", bmpInfoHeader->ciexyzXGreen);
    printf("yGreen             = %d \n", bmpInfoHeader->ciexyzYGreen);
    printf("zGreen             = %d \n", bmpInfoHeader->ciexyzZGreen);
    printf("xBlue             = %d \n", bmpInfoHeader->ciexyzXBlue);
    printf("yBlue             = %d \n", bmpInfoHeader->ciexyzYBlue);
    printf("zBlue             = %d \n", bmpInfoHeader->ciexyzZBlue);
    printf("gammaRed           = %d \n", bmpInfoHeader->gammaRed);
    printf("gammaGreen         = %d \n", bmpInfoHeader->gammaGreen);
    printf("gammaBlue          = %d \n", bmpInfoHeader->gammaBlue);
    printf("intent             = %d \n", bmpInfoHeader->intent);
    printf("profileData        = %d \n", bmpInfoHeader->profileData);
    printf("profileSize        = %d \n", bmpInfoHeader->profileSize);
    printf("reserved           = %d \n", bmpInfoHeader->reserved);


    return fp;
}

unsigned char** readImageData(FILE *fp, BITMAPFILEHEADER *bmpFileHeader, BITMAPINFOHEADER *bmpInfoHeader)
{
    unsigned char **data = NULL;
    int rowSize, pixelArray;
    RGB *pixel;

    rowSize = (((bmpInfoHeader->bitPerPixel * bmpInfoHeader->width) + 31) / 32) * 4;
    pixelArray = rowSize * bmpInfoHeader->height;

    data = createBuffer(bmpInfoHeader->width, bmpInfoHeader->height, bmpInfoHeader->bitPerPixel);

    if(data != NULL)
    {
        int i, j,pad=0;
        pixel = (RGB*)malloc(sizeof(RGB));
        
        fseek(fp, bmpFileHeader->offbits, SEEK_SET);

        for(i=bmpInfoHeader->height-1;i>0;i--)
        {
            for(j=0;j<bmpInfoHeader->width;j+=4)
            {
                fread(pixel, sizeof(RGB), 1, fp);
                data[i][j] = pixel->blue;
                data[i][j+1] = pixel->green;
                data[i][j+2] = pixel->red;
                data[i][j+3] = pixel->alpha;
            }
        }
        // while(i < pixelArray)
        // {
        //     fread(pixel, sizeof(RGB), 1, fp);
        //     data[i] = pixel->blue;
        //     data[i+1] = pixel->green;
        //     data[i+2] = pixel->red;
        //     data[i+3]   = pixel->alpha;
        //     i+=sizeof(pixel);
        // }
        //fclose(fp);
        return data;
    }
    else
    {
        printf("No se pudo asignar memoria para leer los datos de la imagen.\n");
        exit(1);
    }

}

unsigned char** createBuffer(int width, int height, int bitPerPixel)
{
    unsigned char** data = NULL;
    int rowSize, pixelArray, i;

    rowSize = (((bitPerPixel * width) + 31) / 32) * 4;
    printf("ROWSIZE: %d\n", rowSize);

    pixelArray = rowSize * height;
    printf("PIXELARRAY: %d\n", pixelArray);

    data = (unsigned char**)malloc(sizeof(unsigned char*) * height);
    for(i=0; i< height; i++)
        data[i] = (unsigned char*)malloc(sizeof(unsigned char) * rowSize);


    if(data == NULL)
    {
        printf("No hay espacio para los datos de la imagen.\n");
        exit(1);
    }
    else
    {
        return data;
    }
}

unsigned int* binaryImageData(int uflag, unsigned char** data, BITMAPFILEHEADER *bmpFileHeader,BITMAPINFOHEADER *bmpInfoHeader)
{
    unsigned int* binaryData = NULL;
    int rowSize, pixelArray, i, j, k;
    double scale;

    rowSize = (((bmpInfoHeader->bitPerPixel * bmpInfoHeader->width) + 31) / 32) * 4;
    pixelArray = rowSize * bmpInfoHeader->height;

    binaryData = (unsigned int*)malloc(sizeof(unsigned int) * (bmpInfoHeader->height * bmpInfoHeader->width));

    if(binaryData != NULL)
    {
        k = 0;
        
        for(i=0;i<bmpInfoHeader->height;i++)
        {
            for(j=0;j<rowSize;j+=4)
            {
                int red = data[i][j+2];
                int green = data[i][j+1];
                int blue = data[i][j];

                printf("red: %i, green: %i, blue: %i\n", red, green, blue);
                // memcpy((int*)&red, (unsigned char*)&data[i][j+2],2);
                // memcpy((int*)&green, (unsigned char*)&data[i][j+1],2);
                // memcpy((int*)&blue, (unsigned char*)&data[i][j],2);

                scale = red*0.3 + green*0.59 + blue*0.11;

                if(scale>uflag)
                {
                    binaryData[k] = 1;
                }
                else
                {
                    binaryData[k] = 0;
                }

                k++;
            }
        }
           
        

        return binaryData;
    }
    else
    {
        printf("No se pudo asignar memoria para el arreglo binario de pixeles.\n");
        exit(1);
    }
}

void writeBinaryImage(unsigned int* binaryData, int imgCount, BITMAPFILEHEADER *bmpFileHeader, BITMAPINFOHEADER *bmpInfoHeader)
{
    FILE *fp = NULL;

    char fileNumber[5];
    char fileName[30] = "imagenes/resultado_imagen_";
    
    char value[0] = "";

    unsigned char header[14];
    unsigned char infoHeader[108]; 

    sprintf(fileNumber, "%d", imgCount);
    strcat(fileName, fileNumber);
    strcat(fileName, ".bmp");

    if((fp=fopen(fileName, "wb")) == NULL)
    {
        printf("No se logro abrir el archivo: %s.\n", fileName);
        abort();   
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
    
    unsigned int *datitos = (int*)calloc((bmpInfoHeader->size), sizeof(int));
    int rowSize = (((bmpInfoHeader->bitPerPixel * bmpInfoHeader->width) + 31) / 32) * 4;
    int pixelArray = rowSize * bmpInfoHeader->height;

    RGB* pixel = (RGB*)malloc(sizeof(RGB));

    int i,j, k=0, max = 255, min = 0;
    for(i=0;i<bmpInfoHeader->height;i++)
    {
        for(j=0;j<bmpInfoHeader->width;j++)
        {
            //printf("bin %i", binaryData[k]);
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
    }

    //fclose(fp);
}