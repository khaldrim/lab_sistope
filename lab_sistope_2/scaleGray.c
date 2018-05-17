#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/poll.h>

unsigned char** createBuffer(int width, int height, int bitPerPixel);


int main(int argc, char* argv[])
{
    struct pollfd fds[1];
    int pollstatus;
    int cflag, uflag, nflag, i, j, totalData, totalSize;
    unsigned int bpp;
    unsigned long long width, height;

    unsigned char red, green, blue;
    double scale;
    unsigned int* scaleData;
    char buff[9];
    
    printf("        # Inicio scaleGray => pid(%i) \n", getpid());
    
    read(STDOUT_FILENO, &cflag, sizeof(cflag));
    read(STDOUT_FILENO, &uflag, sizeof(uflag));
    read(STDOUT_FILENO, &nflag, sizeof(nflag));
    read(STDOUT_FILENO, &width, sizeof(width));
    read(STDOUT_FILENO, &height, sizeof(height));
    read(STDOUT_FILENO, &bpp, sizeof(bpp));

    totalSize = width * height;
    scaleData = (unsigned int*)malloc(sizeof(unsigned int) * totalSize);
    totalData = totalSize * 3;

    printf("            Inicio de leer datos.\n");
    j = 0;
    for(i = 0; i < totalData; i+=3)
    {
        read(STDOUT_FILENO, &blue, sizeof(unsigned char));
        read(STDOUT_FILENO, &green, sizeof(unsigned char));
        read(STDOUT_FILENO, &red, sizeof(unsigned char));

        /* Mientras leo, convierto los datos a escala de grises */
        scale = (int)red*0.3 + (int)green*0.59 + (int)blue*0.11;
        scaleData[j] = (int)scale;
        j++;
    }
    printf("            Termino de leer todos los datos en scaleGray.\n");
    // printf("        desde scaleGray => cflag: %i | uflag: %i | nflag: %i \n",cflag, uflag, nflag);
    // printf("        desde scaleGray: width %llu  height %llu\n", width,height);

    printf("        # Fin scaleGray\n");
    return 0;

    // fds[0].fd = STDIN_FILENO;
    // fds[0].events = POLLIN | POLLOUT;

    // //                file descrip. , num estructuras, tiempo 
    // pollstatus = poll(fds, 1, 5000);
    // if(pollstatus > 0)
    // {
    //     if(fds[0].revents & POLLIN)
    //     {
    //         read(STDIN_FILENO, &buff, sizeof(buff));
    //     }
    // }

    // printf("        cflag: %i | uflag: %i | nflag: %i \n",cflag, uflag, nflag);
    // printf("        desde el poll: %s\n", buff);

    // printf("        # Fin scaleGray => pid(%i) \n", getpid());
    // return 0;
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