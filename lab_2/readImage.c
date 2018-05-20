#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "struct.h"
#include "bmp.h"

#define READ 0  /* Index of the read end of a pipe */
#define WRITE 1 /* Index of he write end of a pipe*/

/* Cabecera de funciones */
FILE* readImageHeader(int imgCount, FILE* fp, BITMAPFILEHEADER *bmpFileHeader, BITMAPINFOHEADER *bmpInfoHeader);
unsigned char** readImageData(FILE *fp, BITMAPFILEHEADER *bmpFileHeader, BITMAPINFOHEADER *bmpInfoHeader);
unsigned char** createBuffer(int width, int height, int bitPerPixel);

/*
 * Descripcion: Primero inicia el pipe de comunicacion con el hijo, luego utilizamos fork() , en el hijo duplica con dup2() 
 *              la entrada estandar, para que sea utilizada por el siguiente proceso. En el padre primero leemos los que nos llega
 *              del proceso anterior, se leen en orden y se asignan a las variables correspondientes. Luego se lee la cabecera del archivo y
 *              los datos. Despues se escriben en el pipe los datos correspondientes para que los utilice el siguiente proceso.
 * 
 * Entrada: Por argumentos niguna, por entrada estandar llega: cflag, uflag, nflag, bflag
 * 
 * Salida: Hacia el siguiente proceso se envia por pipe: cflag, uflag, nflag, bflag, width, height, offset, pixelData
 */
int main(int argc, char *argv[])
{
    pid_t pid;
    int pipefd[2];
    int status;

    if(pipe(pipefd) == -1)
    {
        printf("Error creando el pipe en readImage.\n");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if(pid == -1)
    {
        /* Error */
        printf("Error creando el fork en el readImage.\n");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        /* Proceso hijo */
        int dupStatus;

        close(pipefd[WRITE]);
        dupStatus = dup2(pipefd[READ], STDIN_FILENO);
        if(dupStatus == -1)
        {
            perror("Dup2 Error: ");
            exit(EXIT_FAILURE);
        }

        execv("./scaleGray", (char *[]){NULL});

        printf("Error al ejecutar el execv desde readImage.\n");
        exit(EXIT_FAILURE);
    }
    else 
    {
        /* Proceso padre */
        int cflag, uflag, nflag,bflag,i,j, rowSize;

        FILE *fp = NULL;
        BITMAPFILEHEADER *bmpFileHeader = NULL;
        BITMAPINFOHEADER *bmpInfoHeader = NULL;
        DATA* data = NULL;

        read(STDIN_FILENO, &cflag, sizeof(int));
        read(STDIN_FILENO, &uflag, sizeof(int));
        read(STDIN_FILENO, &nflag, sizeof(int));
        read(STDIN_FILENO, &bflag, sizeof(int));

        bmpFileHeader = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
        bmpInfoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
        data = (DATA*)malloc(sizeof(DATA));

        fp = readImageHeader(cflag, fp, bmpFileHeader, bmpInfoHeader);
        data->pixelData = readImageData(fp, bmpFileHeader, bmpInfoHeader);
        
        close(pipefd[READ]);
        write(pipefd[WRITE], &cflag, sizeof(int));
        write(pipefd[WRITE], &uflag, sizeof(int));
        write(pipefd[WRITE], &nflag, sizeof(int));
        write(pipefd[WRITE], &bflag, sizeof(int));
        write(pipefd[WRITE], &(bmpInfoHeader->width), sizeof(unsigned long long));
        write(pipefd[WRITE], &(bmpInfoHeader->height), sizeof(unsigned long long));
        write(pipefd[WRITE], &(bmpFileHeader->offbits), sizeof(unsigned int));

        /* Writing image data in pipe*/
        rowSize = bmpInfoHeader->width * 4;

        for(i=bmpInfoHeader->height-1;i>0;i--)
        {
            for(j=0;j<rowSize;j++)
            {
                write(pipefd[WRITE], &data->pixelData[i][j], sizeof(unsigned char));
            }
        }

        wait(&pid);
        return 0;
    }
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
    int rowSize;
    RGB *pixel;

    rowSize = (((bmpInfoHeader->bitPerPixel * bmpInfoHeader->width) + 31) / 32) * 4;

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
        perror("createBuffer => data pointer");
        exit(1);
    }
}