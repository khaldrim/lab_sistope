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
        // dup2(pipefd[READ], STDIN_FILENO);
        // close(pipefd[WRITE]);

        execv("./scaleGray", (char *[]){NULL});

        printf("Error al ejecutar el execv desde Main.\n");
        exit(EXIT_FAILURE);
    }
    else 
    {
        /* Proceso padre */
        int cflag, uflag, nflag;
        struct pollfd fds[1];
        int pollstatus;

        FILE *fp = NULL;
        BITMAPFILEHEADER *bmpFileHeader = NULL;
        BITMAPINFOHEADER *bmpInfoHeader = NULL;
        DATA* data = NULL;

        printf("    # Inicio readImage => pid(%i) \n", getpid());

        read(STDOUT_FILENO, &cflag, sizeof(cflag));
        read(STDIN_FILENO, &uflag, sizeof(uflag));
        read(STDIN_FILENO, &nflag, sizeof(nflag));

        fp = readImageHeader(cflag, fp, bmpFileHeader, bmpInfoHeader);
        data->pixelData = readImageData(fp, bmpFileHeader, bmpInfoHeader);

        printf("     Llega cflag: %i | uflag: %i | nflag: %i \n", cflag, uflag, nflag);
        printf("     Llega a readImage => width: %llu | height: %llu\n", bmpInfoHeader->width, bmpInfoHeader->height);
        
        wait(NULL);
        return 0;
        
        // Aqui escribir archivo en blanco.

        write(pipefd[WRITE], &cflag, sizeof(cflag));
        write(pipefd[WRITE], &uflag, sizeof(uflag));
        write(pipefd[WRITE], &nflag, sizeof(nflag));
        
        fds[0].fd = pipefd[WRITE];
        fds[0].events = POLLIN | POLLOUT;

        // //                file descrip. , num estructuras, tiempo 
        // pollstatus = poll(fds, 1, 5000);
        // if(pollstatus > 0)
        // {
        //     if(fds[0].revents & POLLOUT)
        //     {
        //         write(pipefd[WRITE], "HOLA SAAS", 9);
        //     }
        // }

        printf("    # Fin readImage.\n");
        wait(NULL);
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

    if((fp = fopen(fileName,"rb")) == NULL)
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
        printf("No hay espacio para los datos de la imagen.\n");
        exit(1);
    }
}

