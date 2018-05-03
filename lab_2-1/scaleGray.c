#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "struct.h"

#define READ 0  /* Index of the read end of a pipe */
#define WRITE 1 /* Index of he write end of a pipe*/

int main(int argc, char** argv)
{
    printf("    PID READIMAGE: %i\n",getpid());
    printf("    2. Proceso: scaleGray -> Inicia su proceso.\n");
    
    // int pipeInfo[2];
    BITMAPINFOHEADER *bmp = NULL;

    bmp = argv[0];
    // pipe(pipeInfo);

    // close(pipeInfo[WRITE]);
    // read(pipeInfo[READ], &bmp, sizeof(BITMAPINFOHEADER));
    // close(pipeInfo[READ]);
    
    printf("\n\n ### estoy en scaleGray y el width es: %llu ###\n\n",bmp->width);
    
    // unsigned int* grayData = NULL;
    // int totalSize,rowSize, i, j, k, red, green,blue;
    // double scale;

    // rowSize = (((bmpInfoHeader->bitPerPixel * bmpInfoHeader->width) + 31) / 32) * 4;
    // totalSize = (bmpInfoHeader->height * bmpInfoHeader->width);
    // grayData = (unsigned int*)malloc(sizeof(unsigned int) * totalSize);

    // if(grayData != NULL)
    // {
    //     k = 0;
    //     for(i=bmpInfoHeader->height-1;i>0;i--)
    //     {
    //         for(j=0;j<rowSize;j+=4)
    //         {
    //             red = data[i][j+2];
    //             green = data[i][j+1];
    //             blue = data[i][j];

    //             scale = red*0.3 + green*0.59 + blue*0.11;
    //             grayData[k] = scale;
    //             k++;
    //         }
    //     }
    //     return grayData;
    // }
    // else
    // {
    //     printf("No se pudo asignar memoria para el arreglo binario de pixeles.\n");
    //     exit(1);
    // }
}