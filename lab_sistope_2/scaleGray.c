#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

unsigned char** createBuffer(int width, int height, int bitPerPixel);


int main(int argc, char* argv[])
{
    printf("hola\n");
    return 0;

    
    // pid_t pid;
    // int pipefd[2];
    // int status = 0;

    // if(pipe(pipefd) == -1)
    // {
    //     printf("Error creando el pipe en readImage.\n");
    //     exit(EXIT_FAILURE);
    // }

    // pid = fork();
    // if(pid == -1)
    // {
    //     /* Error */
    //     printf("Error creando el fork en el readImage.\n");
    //     exit(EXIT_FAILURE);
    // }
    // else if(pid == 0)
    // {
    //     /* Proceso hijo */
    //     int dupStatus;

    //     printf("asdaasda\n");
    //     exit(0);

    //     close(pipefd[WRITE]);
    //     dupStatus = dup2(pipefd[READ], STDIN_FILENO);
    //     if(dupStatus == -1)
    //     {
    //         perror("Dup2 Error: ");
    //         exit(EXIT_FAILURE);
    //     }
        
    //     execv("./binaryImage", (char *[]){NULL});

    //     printf("Error al ejecutar el execv desde readImage.\n");
    //     exit(EXIT_FAILURE);
    // }
    // else
    // {
    //     /* Proceso padre */
    //     int cflag, uflag, nflag, bflag, i, j, totalData, totalSize;
    //     unsigned long long width, height;
    //     unsigned char red, green, blue;
    //     double scale;
    //     unsigned int* scaleData;

    //     printf("        # Inicio scaleGray => pid(%i) \n", getpid());
        
    //     read(STDOUT_FILENO, &cflag, sizeof(int));
    //     read(STDOUT_FILENO, &uflag, sizeof(int));
    //     read(STDOUT_FILENO, &nflag, sizeof(int));
    //     read(STDOUT_FILENO, &bflag, sizeof(int));
    //     read(STDOUT_FILENO, &width, sizeof(int));
    //     read(STDOUT_FILENO, &height, sizeof(int));

    //     // fflush(STDIN_FILENO);

    //     printf("            # scaleGray cflag: %i | uflag: %i | nflag: %i | bflag: %i \n", cflag, uflag, nflag, bflag);
    //     printf("            # scaleGray width: %llu | height: %llu \n", width, height);
    //     totalSize = width * height;
    //     scaleData = (unsigned int*)malloc(sizeof(unsigned int) * totalSize);
    //     totalData = totalSize * 3;

    //     // printf("            Inicio de leer datos.\n");
    //     j = 0;
    //     for(i = 0; i < totalData; i+=3)
    //     {
    //         read(STDOUT_FILENO, &blue, sizeof(unsigned char));
    //         read(STDOUT_FILENO, &green, sizeof(unsigned char));
    //         read(STDOUT_FILENO, &red, sizeof(unsigned char));

    //         /* Mientras leo, convierto los datos a escala de grises */
    //         scale = (int)red*0.3 + (int)green*0.59 + (int)blue*0.11;
    //         scaleData[j] = (int)scale;
    //         j++;
    //     }

    //     close(pipefd[READ]);
    //     write(pipefd[WRITE], &cflag, sizeof(int));
    //     write(pipefd[WRITE], &uflag, sizeof(int));
    //     write(pipefd[WRITE], &nflag, sizeof(int));
    //     write(pipefd[WRITE], &bflag, sizeof(int));
    //     write(pipefd[WRITE], &width, sizeof(unsigned long long));
    //     write(pipefd[WRITE], &height, sizeof(unsigned long long));

    //     j = 0;
    //     for(j=0;j<totalSize;j++)
    //     {
    //         write(pipefd[WRITE], &scaleData[j], sizeof(unsigned int));
    //     }

    //     printf("        # Fin scaleGray\n");
    //     wait(&status);
    //     return 0;
    // }
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