#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "struct.h"
#include "function.h"

#define READ 0;  /* Index of the read end of a pipe */
#define WRITE 1; /* Index of he write end of a pipe*/

/*
 * Descripcion: La funcion recibe los parametros ingresados al momento de ejecutar el programa por consola,
 *              utiliza el parametro cflag para realizar un loop en el cual se va llamando a cada imagen con
 *              el nombre "imagen_x.bmp" que se encuentre en la carpeta 'imagenes'. Luego se pide memoria para
 *              crear las estructuras que almacenan los datos de archivo e informacion cabecera de la imagen. 
 *              Se realiza un llamado a la funcion 'readImageHeader' para leer los datos de cabezera del 
 *              archivo bmp, y la informacion de cabecera. Luego se llama a la funcion 'readImageData' para 
 *              leer los pixeles de la imagen. Luego se llama a la funcion 'binaryImageData' para generar un
 *              arreglo que contiene la binarizacion de cada pixel de la imagen, para luego escribir la imagen
 *              binarizada con la funcion 'writeBinaryImage'. Despues, se llama a la funcion 'isNearlyBlack'
 *              para determinar si la imagen es o no 'Nearly black'. El contador de imagenes (imgCount) aumenta, y 
 *              el contador de cantidad de imagenes (Cvalue) disminuye para terminar el ciclo.
 *              Una vez terminado el ciclo se consulta si 'bflag' posee un valor 1 para imprimir por pantalla
 *              los resultados de cada imagen.
 * 
 * Entrada: Parametros solicitados en el enunciado:
 *              cflag -> cantidad de imagenes.
 *              uflag -> Umbral de binarizacion de los pixeles de la imagen.
 *              nflag -> Umbral de porcentaje de pixeles negros en la imagen.
 *              bflag -> Si esta activo se muestra por pantalla la imagen y si es o no 'nearly black'.
 * 
 * Salida: Vacia.
 */
void mainMenu(int cflag, int uflag, int nflag, int bflag)
{
    /* Variables */
    int cValue, imgCount, status;
    FILE *fp = NULL;
    unsigned char **data     = NULL;
    unsigned int *binaryData = NULL;
    unsigned int *grayData   = NULL;
    BITMAPFILEHEADER *bmpFileHeader = NULL;
    BITMAPINFOHEADER *bmpInfoHeader = NULL;
    int* imgPrintResult = NULL;
    int steps;

    /* Variables para crear pipe*/
    pid_t pid;
    int pipeGo[2], pipeBack[2];
    
    imgPrintResult = (int*)malloc(sizeof(int)*cflag);
    if(imgPrintResult == NULL)
    {
        printf("No se logro asignar memoria para imprimir resultados por pantalla.\n");
        exit(1);
    }

    cValue = cflag;
    steps = 0;
    status = 0;
    imgCount = 1;
    
    /* Inicio el pipe para enviar informacion desde el padre al hijo 
     * Inicio el pipe para recibir informacion desde el hijo al padre */
    if(pipe(pipeGo) == -1 || pipe(pipeBack) == -1)
    {
        printf("No se logro crear los pipes.\n");
        exit(1);
    }

    while(cValue > 0)
    {
        bmpFileHeader = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
        bmpInfoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
        
        while(steps < 5)
        {
            pid = fork();
            if(pid == 0) //Hijo
            {
                if(steps == 0) /* Lector de imagen */
                {
                    printf(" Hijo que lee la imagen pid: %i \n", pid);
                    
                    fp   = readImageHeader(imgCount, fp, bmpFileHeader, bmpInfoHeader); /* Se lee las cabeceras y los datos de la imagen */
                    data = readImageData(fp, bmpFileHeader, bmpInfoHeader);
                    exit(0);
                }
                else if(steps == 1) /* Conversor a gris */
                {
                    printf(" Hijo que convierte a gris \n");
                    grayData = scaleGrayData(data, bmpInfoHeader);
                    exit(0);
                } 
                else if(steps == 2) /* Binarizador de imagen */
                {
                    printf(" Hijo que binariza \n");
                    
                    binaryData = binaryImageData(uflag, grayData, bmpInfoHeader); /* Se binarizan los datos obtenidos */
                    exit(0);
                }
                else if(steps == 3) /* Analista de propiedad */
                {
                    printf(" Hijo que analisa \n");
                    
                    imgPrintResult[imgCount-1] = isNearlyBlack(binaryData, nflag, bmpInfoHeader->width, bmpInfoHeader->height); /* Se decide si es nearly black */
                    exit(0);
                }
                else /* Escribir resultado */
                {
                    printf(" Hijo que escribe \n");


                    writeBinaryImage(binaryData, imgCount, bmpFileHeader,bmpInfoHeader); /* Se escribe la imagen */
                    exit(0);
                }

            }
            else //Padre
            {
                int pid_hijo;
                pid_hijo = wait(&status);
                if(pid_hijo != pid)
                {
                    printf("Algo ha ocurrido al terminar el proceso hijo.\n");
                    exit(1);
                }
            }
        }
        
        steps = 0;
        cValue--;
        imgCount++;
        /* Se libera memoria del doble puntero 'data' */
        freeData(data, bmpInfoHeader->width, bmpInfoHeader->height, bmpInfoHeader->bitPerPixel);
        free(bmpFileHeader);
        free(bmpInfoHeader);
        free(binaryData);
    }

    /* Muestra por pantalla resultado si bflag esta activo*/
    if(bflag == 1)
    {
        printResult(imgPrintResult, cflag);
    }

    free(imgPrintResult);
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

    if((fp = fopen(fileName,"rb")) == NULL)
    {
        printf("No se logro abrir el archivo: %s.\n", fileName);
        exit(1);
    }
    
    bmpFileHeader = ReadBMPFileHeader(fp, bmpFileHeader);
    bmpInfoHeader = ReadBMPInfoHeader(fp, bmpInfoHeader);

    // printf("\n\n # Image Data #\n");
    // printf("File type          = %s\n", bmpFileHeader->type);
    // printf("File size          = %d bytes\n", bmpFileHeader->size);
    // printf("Data offset        = %d bytes\n", bmpFileHeader->offbits);
    // printf("Header Size        = %d bytes\n", bmpInfoHeader->size);
    // printf("Width              = %lld pixels\n", bmpInfoHeader->width);
    // printf("Height             = %lld pixels\n", bmpInfoHeader->height);
    // printf("Planes             = %d\n", bmpInfoHeader->planes);
    // printf("Bits per Pixel     = %d bits/pixel\n", bmpInfoHeader->bitPerPixel);
    // printf("Compression        = %d \n", bmpInfoHeader->compression);
    // printf("Size Image         = %d \n", bmpInfoHeader->sizeImage);
    // printf("xPelsperMeter      = %llu \n", bmpInfoHeader->xPelsPerMeter);
    // printf("yPelsPerMeter      = %llu \n", bmpInfoHeader->yPelsperMeter);
    // printf("used               = %d \n", bmpInfoHeader->used);
    // printf("important          = %d \n", bmpInfoHeader->important);
    // printf("redMask            = %d \n", bmpInfoHeader->redMask);
    // printf("greenMask          = %d \n", bmpInfoHeader->greenMask);
    // printf("blueMask           = %d \n", bmpInfoHeader->blueMask);
    // printf("alphaMask          = %d \n", bmpInfoHeader->alphaMask);
    // printf("csType             = %d \n", bmpInfoHeader->csType);
    // printf("xRed               = %d \n", bmpInfoHeader->ciexyzXRed);
    // printf("yRed               = %d \n", bmpInfoHeader->ciexyzYRed);
    // printf("zRed               = %d \n", bmpInfoHeader->ciexyzZRed);
    // printf("xGreen             = %d \n", bmpInfoHeader->ciexyzXGreen);
    // printf("yGreen             = %d \n", bmpInfoHeader->ciexyzYGreen);
    // printf("zGreen             = %d \n", bmpInfoHeader->ciexyzZGreen);
    // printf("xBlue              = %d \n", bmpInfoHeader->ciexyzXBlue);
    // printf("yBlue              = %d \n", bmpInfoHeader->ciexyzYBlue);
    // printf("zBlue              = %d \n", bmpInfoHeader->ciexyzZBlue);
    // printf("gammaRed           = %d \n", bmpInfoHeader->gammaRed);
    // printf("gammaGreen         = %d \n", bmpInfoHeader->gammaGreen);
    // printf("gammaBlue          = %d \n", bmpInfoHeader->gammaBlue);
    // printf("intent             = %d \n", bmpInfoHeader->intent);
    // printf("profileData        = %d \n", bmpInfoHeader->profileData);
    // printf("profileSize        = %d \n", bmpInfoHeader->profileSize);
    // printf("reserved           = %d \n", bmpInfoHeader->reserved);

    return fp;
}

/*
 * Descripcion: La funcion primero realiza el calculo de el tamaño de las filas 'rowSize', luego este valor se multiplica
 *              por la altura 'height' para obtener la cantidad total de bytes que posee la imagen. Luego se crea la matriz
 *              de datos llamado 'data' mediante la funcion 'createBuffer'. Si es posible obtener memoria para los datos de
 *              la imagen se pide memoria para crear el puntero a la estructura 'RGB' llamado 'pixel'. Mediante un 'fseek'
 *              nos posicionamos en donde comienza la matriz de pixeles. Despues mediantes dos ciclos for recorremos la matriz
 *              de datos de la imagen desde la esquina inferior izquierda hasta la esquina superior derecha, entonces se lee
 *              el pixel mediante un 'fread' y los datos obtenidos se guardan en la matriz 'data'. Una vez completado este proceso
 *              se retorna la matriz 'data'
 * 
 * Entrada: Puntero a la imagen 'fp', Puntero a la estructura BITMAPFILEHEADER 'bmpFileHeader', 
 *          Puntero a la estructura BITMAPINFOHEADER 'bmpInfoHeader'.
 * 
 * Salida: Doble puntero a la matriz de datos llamado 'data'.
 */
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

}

/*
 * Descripcion: Esta funcion recibe los datos de altura, ancho y bits por pixel de la imagen para crear
 *              una matriz que sea capaz de almacenar los datos de los pixeles de la imagen. Se pide 
 *              memoria para crear la matriz de datos 'data', si no es posible crear la matriz se detiene el programa.
 *              se retornar el puntero a la matriz de datos 'data'.
 * 
 * Entrada: Entero ancho 'width', Entero alto 'height', Entero bit por pixel 'bitPerPixel'.
 * 
 * Salida: Doble puntero de datos 'data'.
 */
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

/*
 * Descripcion: Esta funcion primero pide memoria para crear un arreglo de enteros del tamaño total de los datos de la imagen,
 *              si no es posible solicitar tal cantidad de memoria el programa se detiene. Luego se recorre la matriz de datos
 *              llamado 'data', rescatando los datos BGR, para luego aplicar la formula solicitada en el enunciado.
 *              Si el resultado 'scale' es mayor a 'uflag' que contiene el umbral ingresado por parametro al iniciar el programa
 *              es mayor, se guarda un 1 en 'binaryData' sino un 0. Se entiende que si es un 1, el pixel se acerca mas al color
 *              blanco, si es un 0 se acerca mas al negro.
 * 
 * Entrada: Entero 'uflag' (Parametro al ejecutar el programa), Doble puntero a char 'data', Puntero a la estructura 
 *          BITMAPFILEHEADER 'bmpFileHeader', Puntero a la estructura BITMAPINFOHEADER 'bmpInfoHeader'.
 * 
 * Salida: Puntero al arreglo de los datos binarizados 'binaryData'.
 */
unsigned int* binaryImageData(int uflag, unsigned int* grayData,BITMAPINFOHEADER *bmpInfoHeader)
{
    unsigned int* binaryData = NULL;
    int totalSize, k;

    totalSize = (bmpInfoHeader->height * bmpInfoHeader->width);
    binaryData = (unsigned int*)malloc(sizeof(unsigned int) * totalSize);

    if(binaryData != NULL)
    {
        k = 0;
        while(k<totalSize)
        {
            if(grayData[k] > uflag)
            {
                binaryData[k] = 1;
            }
            else
            {
                binaryData[k] = 0;
            }

            k++;
        }

        return binaryData;
    }
    else
    {
        printf("No se pudo asignar memoria para el arreglo binario de pixeles.\n");
        exit(1);
    }
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
    while(k < totalSize -1)
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

/*
 * Descripcion: Esta fucion permite liberar la memoria que se requirio para guardar los datos de los pixeles
 *              de la imagen.
 * 
 * Entrada: Doble puntero a char 'data', Entero ancho 'width', Entero largo 'height', Entero 'bitPerPixel'.
 * 
 * Salida: Vacia.
 */
void freeData(unsigned char** data, int width, int height, int bitPerPixel)
{
    int rowSize, pixelArray, i;

    rowSize = (((bitPerPixel * width) + 31) / 32) * 4;
    pixelArray = rowSize * height;

    for(i=0;i<height;i++)
    {
        free(data[i]);
    }

    free(data);
}

/*
 * Descripcion: Funcion que recibe los datos binarizados y el ciclo cuenta la cantidad de 0 en el arreglo
 *              (Recordar que ese 0 representa que un pixel se escalo a negro). Luego realiza una division
 *              para calcuar el porcentaje de cuantos pixeles negros posee la imagen, asi se compara si la 
 *              imagen tiene una mayor cantidad de pixeles negros comparados con el umbral ingresado en 'nflag'.
 *              Se retorna un 1 si se decide que es 'nearlyblack', sino se retorna un 0.
 * 
 * Entrada: Puntero arreglo de enteros 'binaryData', Entero parametro 'nflag', Entero ancho 'width', Entero largo 'height'.
 * 
 * Salida: Entero.
 */
int isNearlyBlack(unsigned int *binaryData, int nflag, int width, int height)
{
    int i, totalSize, black=0;
    float value;

    totalSize = width * height;
    for(i=0;i<totalSize;i++)
    {
        if(binaryData[i] == 0)
        {
            black++;
        }
    }

    value = ((float)black/(float)totalSize) * 100; 
    if( value >  nflag)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 * Descripcion: Funcion que imprime por pantalla si 'imagen_x' es 'nearly black' o no.
 * 
 * Entrada: Puntero a arreglo de enteros 'imgPrintResult', Entero 'cflag'.
 * 
 * Salida: Vacia.
 */ 
void printResult(int* imgPrintResult, int cflag)
{
    int i = 0;

    printf("| Imagen           | NearlyBlack          |\n");
    printf("-------------------------------------------\n");
    
    while(i < cflag)
    {
        if(imgPrintResult[i] == 1)
            printf("| imagen_%i         | Yes                  |\n", i+1);
        else
            printf("| imagen_%i         | No                   |\n", i+1);

        i++;
    }
}

unsigned int* scaleGrayData(unsigned char** data, BITMAPINFOHEADER *bmpInfoHeader)
{
    unsigned int* grayData = NULL;
    int totalSize,rowSize, i, j, k, red, green,blue;
    double scale;

    rowSize = (((bmpInfoHeader->bitPerPixel * bmpInfoHeader->width) + 31) / 32) * 4;
    totalSize = (bmpInfoHeader->height * bmpInfoHeader->width);
    grayData = (unsigned int*)malloc(sizeof(unsigned int) * totalSize);

    if(grayData != NULL)
    {
        k = 0;
        for(i=bmpInfoHeader->height-1;i>0;i--)
        {
            for(j=0;j<rowSize;j+=4)
            {
                red = data[i][j+2];
                green = data[i][j+1];
                blue = data[i][j];

                scale = red*0.3 + green*0.59 + blue*0.11;
                grayData[k] = scale;
                k++;
            }
        }
        return grayData;
    }
    else
    {
        printf("No se pudo asignar memoria para el arreglo binario de pixeles.\n");
        exit(1);
    }
}