#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

/*
 * Descripcion: Recibe los datos del proceso anterior por la entrada estandar, primero los lee y asigna. Luego lee dato por dato convertido a gris, y decide mediante
 *              el umbral uflag si es un 1 o 0. Este dato se guarda en el arreglo de enteros binarios. Luego se escribe por el pipe para enviar los datos al siguiente proceso.
 * 
 * Entrada: Por argumento nada, por la entrada estandar: cflag, uflag, nflag, bflag, width, height, offset, datos de los pixeles convertidos a grises.
 * 
 * Salida: Por el pipe: cflag, nflag, bflag, width, height, offset, datos de los pixeles binarizados. 
*/
int main(int argc, char* argv[])
{
    pid_t pid;
    int pipefd[2];

    if(pipe(pipefd) == -1)
    {
        printf("Error creando el pipe en binaryImage.\n");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if(pid == -1)
    {
        /* Error */
        printf("Error creando el fork en el binaryImage.\n");
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

        execv("./analisisImage", (char *[]){NULL});

        printf("Error al ejecutar el execv desde readImage.\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        int cflag, uflag, nflag, bflag, totalData, totalSize, i;
        unsigned long long width, height;
        unsigned int offbits, data;
        unsigned int* binaryData; 

        read(STDIN_FILENO, &cflag, sizeof(int));
        read(STDIN_FILENO, &uflag, sizeof(int));
        read(STDIN_FILENO, &nflag, sizeof(int));
        read(STDIN_FILENO, &bflag, sizeof(int));
        read(STDIN_FILENO, &width, sizeof(unsigned long long));
        read(STDIN_FILENO, &height, sizeof(unsigned long long));
        read(STDIN_FILENO, &offbits, sizeof(unsigned int));

        totalSize = width * height;
        binaryData = (unsigned int*)malloc(sizeof(unsigned int) * totalSize);
        totalData = totalSize * 3;

        for(i = 0; i < totalSize; i++)
        {
            read(STDIN_FILENO, &data, sizeof(unsigned int));

            /* Binarizo los datos que llegan desde scaleGray */
            if(data>uflag)
                binaryData[i] = 1;
            else
                binaryData[i] = 0;
        }

        close(pipefd[READ]);
        write(pipefd[WRITE], &cflag, sizeof(int));
        write(pipefd[WRITE], &nflag, sizeof(int));
        write(pipefd[WRITE], &bflag, sizeof(int));
        write(pipefd[WRITE], &width, sizeof(unsigned long long));
        write(pipefd[WRITE], &height, sizeof(unsigned long long));
        write(pipefd[WRITE], &offbits, sizeof(unsigned int));

        for(i=0;i<totalSize;i++)
            write(pipefd[WRITE], &binaryData[i], sizeof(unsigned int));

        wait(&pid);
        return 0;
    }
}