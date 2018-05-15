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
    // BITMAPFILEHEADER *bmpFileHeader;
    // int arg[2];
    // int fd;

    // fd = atoi(argv[0]);
    // dup2(arg, fd);
    
    // close(arg[WRITE]); 

    // read(arg[READ], &bmpFileHeader, sizeof(BITMAPFILEHEADER));
    // close(arg[READ]);

    printf("hola desde scalegray\n");
    // printf("fileheader size -> %i\n", bmpFileHeader->size);

    
    return 0;
}