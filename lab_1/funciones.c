#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "struct.h"
#include "encabezados.h"

/*
 * Input      : 
 * Output     : 
 * Description: 
 */
int* binaryData(int uflag, unsigned char **data, BMPINFOWINHEADER *bmpIH)
{
    int i, j,totalSize;
    long scale;
    int k=0;
    int* binaryData;

    totalSize = bmpIH->winHeight * bmpIH->winWidth;
    binaryData = (int*)malloc(sizeof(int) *totalSize);

    if(binaryData == NULL)
    {
        printf("No se pudo asignar memoria para el arreglo binario de pixeles.\n");
        return NULL;
    }

    for(i=0; i< bmpIH->winHeight; i++)
    {
        for(j=0; j < bmpIH->winWidth; j++)
        {
            scale = (((int)data[i][j+2])*0.3) + (((int)data[i][j+1])*0.59) + (((int)data[i][j])*0.11);
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

/*
    Entrada    : Ingresan los valores de las banderas solicitadas como parametros al usuario.
    Salida     : Retorna un entero, con dos valores posibles 1 y 0. Retorna 1 si la funcion cumplio su flujo normal y retorna 0 si existe algun error.
    Descripcion: Funcion que bla bla... 
*/
void mainMenu(int cflag, int uflag, int nflag, int bflag)
{
    int cvalue = 0, imgCount = 1;
    cvalue = cflag;
    
    /* Etapas del Pipeline:
            1. Leer el la informacion de cabezera de un archivo bitmap.
            2. Leer pixel por pixel la imagen, aplicando formula del enunciado.
            3. Determinar si el pixel debe ser transformado a blanco o a negro, dependiendo del umbral.
            4. Clasificar la imagen como 'nearly black' si esta supera el umbral.
            5. Crear la imagen binarizada.
            6. Repetir Pipeline si aun quedan imagenes por leer.      
            7. Imprimir por pantalla la informacion solicitada si la bandera -b esta activa.
    */
    
    while(cvalue > 0)
    {
        FILE *fp                           = NULL;
        unsigned char **data               = NULL;
        int           *binData             = NULL;
        BMPFILEHEADER    *bmpFileHeader    = NULL;
        BMPINFOOSHEADER  *bmpOsInfoHeader  = NULL;
        BMPINFOWINHEADER *bmpWinInfoHeader = NULL;
        RGB *palette                       = NULL;
        RGB *pixel                         = NULL;

        bmpFileHeader    = (BMPFILEHEADER *)   malloc(sizeof(BMPFILEHEADER));
        bmpOsInfoHeader  = (BMPINFOOSHEADER *) malloc(sizeof(BMPINFOOSHEADER));
        bmpWinInfoHeader = (BMPINFOWINHEADER*) malloc(sizeof(BMPINFOWINHEADER));
        
        fp = readImageHeader(imgCount, fp,bmpFileHeader, bmpOsInfoHeader, bmpWinInfoHeader);
        
        if(bmpFileHeader->headersize == 40 || bmpFileHeader->headersize == 124)
        {   
            
            data       = readImageData(fp, bmpWinInfoHeader, bmpFileHeader, palette, pixel);
            binData    = binaryData(uflag, data, bmpWinInfoHeader);
            writeFile(imgCount,binData, bmpWinInfoHeader, bmpFileHeader);

            //writeData(uflag, imgCount,data, imgSize, bmpFileHeader, bmpWinInfoHeader, bmpOsInfoHeader);

            /* Luego, si bflag = 1, se procede a almacenar el resultado de writeData, 
             * para despues mostrar por pantalla cuales imagenes fueron o no 'nearly black'
             */
        }

        cvalue--;
        imgCount++;

        fclose(fp);
        //free(data);
        free(bmpFileHeader);
        free(bmpOsInfoHeader);
        free(bmpWinInfoHeader);
    }
}


unsigned char** createBuffer(int width, int height)
{
    unsigned char** data = NULL;
    int padding = 0;
    int totalWidthSize = 0;
    int i;

    padding = (4 - (width * 3) % 4) % 4;
    totalWidthSize = padding + (width * 3);

    data = (unsigned char**)malloc(sizeof(unsigned char*) * totalWidthSize);

    for(i=0;i<width;i++)
    {
        data[i] = (unsigned char*)malloc(sizeof(unsigned char) * height);
    }

    if(data == NULL)
        return NULL;
    else
        return data;
}

unsigned char** readImageData(FILE *fp, BMPINFOWINHEADER *bmpIH, BMPFILEHEADER *bmpFH, RGB *palette, RGB *pixel)
{
    unsigned char **data = NULL;
    int i, j, pad;
    long scale; 

    //quizas este if este demas
    if(bmpIH->winColorPalette > 0)
    {
        palette = (RGB*)malloc(sizeof(RGB) * bmpIH->winColorPalette);
        if(palette == NULL)
            return NULL;
    }

    data = createBuffer(bmpIH->winWidth, bmpIH->winHeight);

    if(data != NULL)
    {
        pixel = (RGB*)malloc(sizeof(RGB));
        for(j=0;j<bmpIH->winHeight;j++)
        {
            //printf("------ ROW %i --------\n", j);
            pad = 0;
            for(i=0;i<bmpIH->winWidth;i++)
            {
                //if(fread(pixel, 1, sizeof(RGB),fp) != sizeof(RGB))
                //{
                //    printf("Error leyendo los pixeles.\n");
                //    abort();
                //}

                fread(pixel, 1,sizeof(RGB),fp);

                data[j][i]   = pixel->blue;
                data[j][i+1] = pixel->green;
                data[j][i+2] = pixel->red;

                pad += sizeof(RGB);
            }

            if(pad % 4 != 0)
            {
                int z;
                pad = 4 - (pad%4);
                //printf("Padding: %d bytes\n", pad);
                fread(pixel, pad, 1, fp);
                for(z=0; z<pad; z++)
                {
                    data[j][i+z] = 0;
                }
                //data[j][i] = pad;
            } 
        }

        return data;
    }
    else
    {
        printf("No se pudo asignar memoria para leer los datos de la imagen.\n");
        return NULL;
    }


}

void writeFile(int imgCount,int *binaryData, BMPINFOWINHEADER *bmpIH, BMPFILEHEADER *bmpFH)
{
    int i,j,k, pad;
    FILE *fp = NULL;
    RGB *pixel = NULL;
    char fileNumber[5];
    
    char fileName[30] = "imagenes/resultado_imagen_";
    char value[0] = "";

    sprintf(fileNumber, "%d", imgCount);
    strcat(fileName, fileNumber);
    strcat(fileName, ".bmp");

    if((fp=fopen(fileName, "wb")) == NULL)
    {
        printf("No se logro abrir el archivo: %s.\n", fileName);
        abort();   
    }

    /* FILE HEADER */
    fwrite(&bmpFH->fileType, 2, 1, fp);
    fwrite(&bmpFH->fileSize, 4, 1, fp);
    fwrite(&bmpFH->reserved1, 2, 1, fp);
    fwrite(&bmpFH->reserved2, 2, 1, fp);
    fwrite(&bmpFH->offBits, 4, 1, fp);

    if(bmpFH->headersize == 40 || bmpFH->headersize == 124)
    {
        /* WIN INFO HEADER */
        fwrite(&bmpIH->winSize, 4, 1, fp);
        fwrite(&bmpIH->winWidth, 4, 1, fp);
        fwrite(&bmpIH->winHeight, 4, 1, fp);
        fwrite(&bmpIH->winColorPlanes, 2, 1, fp);
        fwrite(&bmpIH->winBitsPerPixel, 2, 1, fp);
        fwrite(&bmpIH->winCompression, 4, 1, fp);
        fwrite(&bmpIH->winImgSize, 4, 1, fp);
        fwrite(&bmpIH->winXPixPerMeter, 4, 1, fp);
        fwrite(&bmpIH->winYPixPerMeter, 4, 1, fp);
        fwrite(&bmpIH->winColorPalette, 4, 1, fp);
        fwrite(&bmpIH->winColorUsed, 4, 1, fp);
    }

    /* IMAGE PIXEL DATA */
        pixel = (RGB*)malloc(sizeof(RGB));
    if(pixel == NULL)
    {
        printf("No se pudo asignar memoria para escribir los pixeles de la imagen.\n");
        exit(1);
    }

    k = 0;
    for(j=0; j<bmpIH->winHeight;j++)
    {
        pad = 0;
        for(i=0; i<bmpIH->winWidth;i++)
        {
            if(binaryData[k] == 1)
            {
                pixel->blue = 255;
                pixel->green = 255;
                pixel->red = 255;
                pixel->alpha = 1;
            }
            else
            {
                pixel->blue = 0;
                pixel->green = 0;
                pixel->red = 0;
                pixel->alpha = 1;
            }

            fwrite((RGB*)pixel, sizeof(RGB),1, fp);

            pad += 4;
            //printf(" w: %ld  h: %ld  k:%ld bin: %ld \n",i,j,k,binaryData[k]);
            k++;
        }

        if(pad % 4 != 0)
        {
            int i , value = 0;
            pad = 4 - (pad % 4);
            for(i=0;i<pad;i++)
                fwrite(&value, 1, 1, fp);
        }


    }
    free(pixel);
    fclose(fp);
}