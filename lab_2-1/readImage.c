#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "readImage.h"
#include "struct.h"

#define READ 0  /* Index of the read end of a pipe */
#define WRITE 1 /* Index of he write end of a pipe*/

int main(int argc, char** argv)
{
    /* Variables que ingresan por parametro */
    int uflag  = 0;
    int nflag  = 0;
    int imgNum = 0;

    imgNum = atoi(argv[0]);
    uflag  = atoi(argv[1]);
    nflag  = atoi(argv[2]);

    printf("        argc: %i\n", argc);
    printf("        i: %d\n", imgNum);
    printf("        u: %d\n", uflag);
    printf("        n: %d\n", nflag);

    /* Variables del proceso */
    int rowSize, pixelArray;
    RGB *pixel;
    BITMAPFILEHEADER *bmpFileHeader = NULL;
    BITMAPINFOHEADER *bmpInfoHeader = NULL;
    DATA *data = NULL;
    FILE *fp = NULL;
    
    char fileNumber[5];
    char fileName[30] = "imagenes/imagen_";
    int pipeFile[2], pipeInfo[2], pipeData[2], pipeUflag[2], pipeNflag[2];
    pid_t pid;
    
    if((pipe(pipeFile)) == -1 || pipe(pipeInfo) == -1 || pipe(pipeData) == -1 || pipe(pipeUflag) == -1 || pipe(pipeNflag) == -1)
    {
        printf("Error creando el pipe de comunicacion del proceso readImage a scaleGray.\n");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if(pid == -1) /* Error */
    {
        printf("No se logro crear el hijo en el proceso que lee la imagen.\n");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0) /* Hijo */
    {
        BITMAPINFOHEADER *info = NULL;
        printf("Soy el hijo! readImage \n");
        printf("pid hijo de readImage: %i\n",getpid());

        //         close(pipeFile[READ]);
        close(pipeInfo[READ]);
        // close(pipeData[READ]);
        // close(pipeUflag[READ]);
        // close(pipeNflag[READ]);

        write(pipeInfo[WRITE], &info, sizeof(BITMAPFILEHEADER));
        close(pipeInfo[WRITE]);

        char buff[10] = "";
        sprintf(buff,"%d",info);
        
        
        char *argv[2] = {buff,NULL}; /* argv to execv*/
        execv("scaleGray",argv);            

    }
    else /* Padre */
    {
        printf("    PID READIMAGE: %i\n",getpid());
        printf("    1. Proceso: readImage -> Inicia su proceso.\n");

        sprintf(fileNumber, "%d", imgNum);
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
        printf("        Archivo abierto correctamente.\n");
        bmpFileHeader = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
        bmpInfoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
        data          = (DATA*)malloc(sizeof(DATA));
        
        bmpFileHeader = ReadBMPFileHeader(fp, bmpFileHeader);
        bmpInfoHeader = ReadBMPInfoHeader(fp, bmpInfoHeader);

        /* Leemos los datos de la imagen */
        printf("        Empezamos a leer los datos de la imagen.\n");
        rowSize = (((bmpInfoHeader->bitPerPixel * bmpInfoHeader->width) + 31) / 32) * 4;
        pixelArray = rowSize * bmpInfoHeader->height;

        data = createBuffer(bmpInfoHeader->width, bmpInfoHeader->height, bmpInfoHeader->bitPerPixel, data);

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
                    data->pixelData[i][j]   = pixel->blue;
                    data->pixelData[i][j+1] = pixel->green;
                    data->pixelData[i][j+2] = pixel->red;
                    data->pixelData[i][j+3] = pixel->alpha;
                }
            }
            fclose(fp);
        }
        printf("        Datos leidos correctamente.\n");

        /* Le envio los datos al hijo */

        

        /* Esperamos a que el hijo termine */
        wait(NULL);
    }

    //printf("    1. Proceso: readImage -> Finaliza su ejecucion.\n");
    return 0;
}

DATA* createBuffer(int width, int height, int bitPerPixel, DATA *data)
{
    int rowSize, pixelArray, i;

    rowSize = (((bitPerPixel * width) + 31) / 32) * 4;
    pixelArray = rowSize * height;
    data->pixelData = (unsigned char**)malloc(sizeof(unsigned char*) * height);

    if(data != NULL)
    {
        for(i=0; i< height; i++)
        {
            data->pixelData[i] = (unsigned char*)malloc(sizeof(unsigned char) * rowSize);
            if(data->pixelData[i] == NULL)
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