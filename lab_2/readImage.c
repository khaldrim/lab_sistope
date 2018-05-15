#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "struct.h"
#include "readImage.h"

#define READ 0  /* Index of the read end of a pipe */
#define WRITE 1 /* Index of he write end of a pipe*/

int main(int argc,char* argv[])
{
    pid_t pid;
    int status;

    // Pipes
    int fdFH[2], fdIH[2], fdData[2];

    pid = fork();
    if(pid == -1)
    {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0) /* Proceso hijo */
    {
        /* scaleGray Call */
        /* Piping to scaleGray
                - bmpFileHeader
                - bmpInfoHeader
                - data
        */

        //    char buffFH[25];
        //    int newfd = dup(fdFH[WRITE]);

        // sprintf(buffFH,"%d",newfd);


        // //Calling execv with next step
        // execv("./scaleGray",(char *[]){ buffFH, NULL});


        printf("Llamo a el 3er proceso del pipe \n");
    }
    else /* Proceso padre*/
    {

        if( (pipe(fdFH) == -1) || (pipe(fdIH) == -1) || (pipe(fdData) == -1))
        {  
            perror("Error");
            exit(EXIT_FAILURE);
        }

        /* Obtengo los parametros de entrada */
        int cflag,uflag,nflag;
        BITMAPFILEHEADER *bmpFileHeader = NULL;
        BITMAPINFOHEADER *bmpInfoHeader = NULL;
        DATA *data;
        FILE *fp;


        bmpFileHeader = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
        bmpInfoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
        data = (DATA*)malloc(sizeof(DATA));

        
        int *pipeCount = atoi(argv[0]);
        pipe(pipeCount);

        close(pipeCount[WRITE]);
        read(pipeCount[READ], &cflag, sizeof(cflag));

        printf("    cflag: %i\n", cflag);
        exit(0);
        // cflag = atoi(argv[0]);
        //uflag = atoi(argv[1]);
        //nflag = atoi(argv[2]);

        //printf("Proceso: %i, recibe -c %i -u %i -n %i\n",getpid(),cflag,uflag,nflag);
        

        fp = readImageHeader(cflag, fp, bmpFileHeader, bmpInfoHeader);
        data->pixelData = readImageData(fp, bmpFileHeader, bmpInfoHeader);

        close(fdFH[READ]);
        // close(fdFH[READ]);
        // close(fdData[READ]);

        write(fdFH[WRITE], &bmpFileHeader, sizeof(BITMAPFILEHEADER));

        close(fdFH[WRITE]);


        waitpid(pid, &status,WUNTRACED);
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