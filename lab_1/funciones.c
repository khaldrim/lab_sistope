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
int* binaryData(int uflag, unsigned char **data, BMPINFOWINHEADER *bmpIH)
{
    int i, j,totalSize, scale;
    int k=0;
    int* binaryData;

    totalSize = bmpIH->winHeight * bmpIH->winWidth;
    binaryData = (int*)malloc(sizeof(int) *totalSize);

    if(binaryData == NULL)
    {
        printf("No se pudo asignar memoria para el arreglo binario de pixeles.\n");
        return NULL;
    }

    for(i=0; i<bmpIH->winHeight; i++)
    {
        for(j=0; j < bmpIH->winWidth; j++)
        {
            scale = (((int)data[i][j])*0.3) + (((int)data[i][j+1])*0.59) + (((int)data[i][j+2])*0.11);
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
        
        if(bmpFileHeader->headersize == 40)
        {   
            
            data       = readImageData(fp, bmpWinInfoHeader, bmpFileHeader, palette, pixel);
            binData    = binaryData(uflag, data, bmpWinInfoHeader);
            writeFile(imgCount,binData, bmpWinInfoHeader);

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
            printf("------ ROW %i --------\n", j);
            pad = 0;
            for(i=0;i<bmpIH->winWidth;i++)
            {
                if(fread(pixel, 1, sizeof(RGB),fp) != sizeof(RGB))
                {
                    printf("Error leyendo los pixeles.\n");
                    abort();
                }

                data[j][i]   = pixel->blue;
                data[j][i+1] = pixel->green;
                data[j][i+2] = pixel->red;

                pad += sizeof(RGB);
                printf("Pixel %d: %3d %3d %3d\n", i, pixel->red, pixel->green, pixel->blue);
            }

            if(pad % 4 != 0)
            {
                pad = 4 - (pad%4);
                printf("Padding: %d bytes\n", pad);
                fread(pixel, pad, 1, fp);

                data[j][i] = pad;
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

void writeFile(int imgCount,int *binaryData, BMPINFOWINHEADER *bmpIH)
{
    int i,j;
    FILE *fp = NULL;
    char fileNumber[5];
    char fileName[30] = "imagenes/resultado_imagen_";
    char value[1] = "";

    sprintf(fileNumber, "%d", imgCount);
    strcat(fileName, fileNumber);
    strcat(fileName, ".txt");

    if((fp=fopen(fileName, "w")) == NULL)
    {
        printf("No se logro abrir el archivo: %s.\n", fileName);
        abort();   
    }

    for(j=0; j<bmpIH->winHeight; j++)
    {
        for(i=0; i<bmpIH->winWidth; i++)
        {
            sprintf(value, "%d", binaryData[i]);
            fwrite(&value, sizeof(unsigned char), 1,fp);
        }

        fwrite("\n", sizeof(char), 1,fp);
        
    }

}