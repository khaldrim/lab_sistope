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
char* readPixelData(FILE *fp, unsigned char *data,int width, int height, int offset)
{
    int size = width*height;
    fseek(fp, offset, SEEK_SET); 
    fread(data, sizeof(unsigned char), (3*size), fp);

    int i;
    for(i = 0; i < (size); i+= 3)
    {
        /* Intercambio BGR a RGB, debido a que asi lo almacena la imagen windows.*/

        unsigned char tmp = data[i];
        data[i] = data[i+2];
        data[i+2] = tmp;

        //printf("N_%i R: %i G: %i B: %i.\n", i, data[i], data[i+1], data[i+2]);
    }

    return data;
}


/*
 * Input      : 
 * Output     : 
 * Description: 
 */
char* scaleGreyData(int uflag, unsigned char *data, unsigned char *binary_data, int size)
{
    int i, j = 0;

    for(i=0;i<size;i+=3)
    {
        float scale = (((int)data[i+2])*0.3) + (((int)data[i+1])*0.59) + (((int)data[i])*0.11);
        if(scale > uflag)
        {
            binary_data[j] = 0;
        }
        else
        {
            binary_data[j] = 1;
        }

        //printf("N_%i Scale: %f \n R: |%i|%f| B: |%i|%f| G: |%i|%f|\n", i,scale, data[i],(data[i]*0.3), data[i+1],(data[i+1]*0.59), data[i+2],(data[i+2]*0.11));
        j++;
    }

    return binary_data;
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
        FILE *fp = NULL;
        unsigned char *data = NULL;
        unsigned char *binary_data = NULL;
        BMPFILEHEADER    *bmpFileHeader    = NULL;
        BMPINFOOSHEADER  *bmpOsInfoHeader  = NULL;
        BMPINFOWINHEADER *bmpWinInfoHeader = NULL;
        
        bmpFileHeader    = (BMPFILEHEADER *) malloc(sizeof(BMPFILEHEADER));
        bmpOsInfoHeader  = (BMPINFOOSHEADER *) malloc(sizeof(BMPINFOOSHEADER));
        bmpWinInfoHeader = (BMPINFOWINHEADER*) malloc(sizeof(BMPINFOWINHEADER));
        
        fp = readImageHeader(imgCount, fp,bmpFileHeader, bmpOsInfoHeader, bmpWinInfoHeader);
        
        if(bmpFileHeader->headersize == 40)
        {   
            int imgSize = (bmpWinInfoHeader->winHeight * bmpWinInfoHeader->winWidth * bmpWinInfoHeader->winBitsPerPixel) / 8;
            printf("\nThe size of the image is: %i\n.",imgSize);
            
            if((data = (char*)malloc(sizeof(char)*imgSize)) == NULL)
            {
                printf("No hay memoria para alojar los datos de la imagen.\n");
                abort();
            }

            fseek(fp, bmpFileHeader->offBits, SEEK_SET);
            fread(data, imgSize, 1, fp);
            writeData(imgCount,data, imgSize, bmpFileHeader, bmpWinInfoHeader);
        }

        cvalue--;
        imgCount++;

        fclose(fp);
        free(data);
        free(binary_data);
        free(bmpFileHeader);
        free(bmpOsInfoHeader);
        free(bmpWinInfoHeader);
    }
}


void writeData(int imgCount, unsigned char *data, int imgSize, BMPFILEHEADER *bmpfh, BMPINFOWINHEADER *bmpWinIH)
{
    FILE *fp = NULL;
    char fileNumber[5];
    char fileName[30] = "imagenes/resultado_imagen_";

    sprintf(fileNumber, "%d", imgCount);
    strcat(fileName, fileNumber);
    strcat(fileName, ".bmp");

    if((fp=fopen(fileName, "wb")) == NULL)
    {
        printf("No se logro abrir el archivo: %s.\n", fileName);
        abort();   
    }

    /* File Header */
    fwrite(&bmpfh->fileType, 2, 1, fp);  /* fileType */
    fwrite(&bmpfh->fileSize, 4, 1, fp);  /* filesize */
    fwrite(&bmpfh->reserved1, 2, 1, fp); /* reserved1  */
    fwrite(&bmpfh->reserved2, 2, 1, fp); /* reserved2 */
    fwrite(&bmpfh->offBits, 4, 1, fp);   /* offBits */

    fwrite(&bmpWinIH->winSize, 4, 1, fp);         /* Size */
    fwrite(&bmpWinIH->winWidth, 4, 1, fp);        /* Width */
    fwrite(&bmpWinIH->winHeight, 4, 1, fp);       /* Height */
    fwrite(&bmpWinIH->winColorPlanes, 2, 1, fp);  /* ColorPlanes */
    fwrite(&bmpWinIH->winBitsPerPixel, 2, 1, fp); /* BitsPerPixel */
    fwrite(&bmpWinIH->winCompression, 4, 1, fp);  /* Compression */
    fwrite(&bmpWinIH->winImgSize, 4, 1, fp);      /* Image Size */
    fwrite(&bmpWinIH->winXPixPerMeter, 4, 1, fp); /* XPixPerMeter */
    fwrite(&bmpWinIH->winYPixPerMeter, 4, 1, fp); /* YPixPerMeter */
    fwrite(&bmpWinIH->winColorPalette, 4, 1, fp); /* ColorPalette */
    fwrite(&bmpWinIH->winColorUsed, 4, 1, fp);    /* ColorUsed */

    printf("OK!\n");
    int i;
    for(i=0; i < imgSize; i++)
    {
        fwrite(&data[i], sizeof(unsigned char), 1, fp);
    }
}

void printData(unsigned char *data, int imgSize)
{
    int i;
    for(i=0; i < imgSize; i++)
        printf("B:%i G:%i R:%i\n", data[i], data[i+1], data[i+2]);
}