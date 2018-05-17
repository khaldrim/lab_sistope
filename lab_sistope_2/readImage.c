#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/poll.h>
#include "struct.h"
#include "readImage.h"

#define READ 0  /* Index of the read end of a pipe */
#define WRITE 1 /* Index of he write end of a pipe*/

int main(int argc, char *argv[])
{
    pid_t pid;
    int pipefd[2];
    int status = 0;

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
        dupStatus = dup2(pipefd[READ], STDOUT_FILENO);
        if(dupStatus == -1)
        {
            perror("Dup2 Error: ");
            exit(EXIT_FAILURE);
        }

        close(pipefd[READ]);
        execv("./scaleGray", (char *[]){NULL});

        printf("Error al ejecutar el execv desde readImage.\n");
        exit(EXIT_FAILURE);
    }
    else 
    {
        /* Proceso padre */
        int cflag, uflag, nflag,i,j, rowSize;
        struct pollfd fds[1];
        int pollstatus;

        FILE *fp = NULL;
        BITMAPFILEHEADER *bmpFileHeader = NULL;
        BITMAPINFOHEADER *bmpInfoHeader = NULL;
        DATA* data = NULL;

        printf("    # Inicio readImage => pid(%i) \n", getpid());

        read(STDOUT_FILENO, &cflag, sizeof(cflag));
        read(STDOUT_FILENO, &uflag, sizeof(uflag));
        read(STDOUT_FILENO, &nflag, sizeof(nflag));

        bmpFileHeader = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
        bmpInfoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
        data = (DATA*)malloc(sizeof(DATA));

        fp = readImageHeader(cflag, fp, bmpFileHeader, bmpInfoHeader);
        data->pixelData = readImageData(fp, bmpFileHeader, bmpInfoHeader);
        
        // // Aqui escribir archivo en blanco.

        close(pipefd[READ]);
        
        write(pipefd[WRITE], &cflag, sizeof(cflag));
        write(pipefd[WRITE], &uflag, sizeof(uflag));
        write(pipefd[WRITE], &nflag, sizeof(nflag));
        write(pipefd[WRITE], &bmpInfoHeader->width, sizeof(bmpInfoHeader->width));
        write(pipefd[WRITE], &bmpInfoHeader->height, sizeof(bmpInfoHeader->height));
        write(pipefd[WRITE], &bmpInfoHeader->bitPerPixel, sizeof(bmpInfoHeader->bitPerPixel));
        /* Writing image data in pipe*/
        rowSize = (((bmpInfoHeader->bitPerPixel * bmpInfoHeader->width) + 31) / 32) * 4;

        for(i=bmpInfoHeader->height-1;i>0;i--)
        {
            for(j=0;j<rowSize;j+=4)
            {
                write(pipefd[WRITE], &data->pixelData[i][j+2], sizeof(unsigned char));
                write(pipefd[WRITE], &data->pixelData[i][j+1], sizeof(unsigned char));
                write(pipefd[WRITE], &data->pixelData[i][j], sizeof(unsigned char));
            }
        }

        close(pipefd[WRITE]);

        // fds[0].fd = pipefd[WRITE];
        // fds[0].events = POLLIN | POLLOUT;

        // // //                file descrip. , num estructuras, tiempo 
        // // pollstatus = poll(fds, 1, 5000);
        // // if(pollstatus > 0)
        // // {
        // //     if(fds[0].revents & POLLOUT)
        // //     {
        // //         write(pipefd[WRITE], "HOLA SAAS", 9);
        // //     }
        // // }

        wait(&status);
        printf("    # Fin readImage.\n");
        return 0;
    }
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

