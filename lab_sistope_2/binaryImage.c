#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

int main(int argc, char* argv[])
{
    pid_t pid;
    int pipefd[2];
    int status = 0;

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
        int cflag, uflag, nflag, bflag, totalData, totalSize, data, i;
        unsigned long long width, height;
        unsigned int* binaryData; 

        printf("            # Inicio binaryImage => pid(%i) \n", getpid());

        /* Leyendo datos enviados desde scaleGray */
        read(STDIN_FILENO, &cflag, sizeof(cflag));
        read(STDIN_FILENO, &uflag, sizeof(uflag));
        read(STDIN_FILENO, &nflag, sizeof(nflag));
        read(STDIN_FILENO, &bflag, sizeof(bflag));
        read(STDIN_FILENO, &width, sizeof(width));
        read(STDIN_FILENO, &height, sizeof(height));

        printf("            # binary cflag: %i | uflag: %i | nflag: %i | bflag: %i \n", cflag, uflag, nflag, bflag);

        totalSize = width * height;
        binaryData = (unsigned int*)malloc(sizeof(unsigned int) * totalSize);
        totalData = totalSize * 3;

        for(i = 0; i < totalData; i++)
        {
            read(STDIN_FILENO, &data, sizeof(int));
            
            /* Binarizo los datos que llegan desde scaleGray */
            if(data>uflag)
            {
                binaryData[i] = 1;
            }
            else
            {
                binaryData[i] = 0;
            }
        }

        close(pipefd[READ]);
        write(pipefd[WRITE], &cflag, sizeof(cflag));
        write(pipefd[WRITE], &nflag, sizeof(nflag));
        write(pipefd[WRITE], &bflag, sizeof(bflag));
        write(pipefd[WRITE], &width, sizeof(width));
        write(pipefd[WRITE], &height, sizeof(height));


        for(i=0;i<totalSize;i++)
            write(pipefd[WRITE], &binaryData[i], sizeof(unsigned int));

        printf("    pipe eescrito\n");

        wait(&status);
        printf("            # Fin binaryImage ");
        return 0;
    }
}