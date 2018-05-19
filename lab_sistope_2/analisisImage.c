#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

int isNearlyBlack(unsigned int *binaryData, int nflag, int width, int height);

int main(int argc, char* argv[])
{
    pid_t pid;
    int pipefd[2];
    int status = 0;

    if(pipe(pipefd) == -1)
    {
        printf("Error creando el pipe en analisisImage.\n");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if(pid == -1)
    {
        /* Error */
        printf("Error creando el fork en el analisisImage.\n");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        /* Proceso hijo */
        int dupStatus;

        printf("analisisimage hijo\n");
        exit(0);
        dupStatus = dup2(pipefd[READ], STDIN_FILENO);
        if(dupStatus == -1)
        {
            perror("Dup2 Error: ");
            exit(EXIT_FAILURE);
        }
        close(pipefd[WRITE]);
        execv("./writeImage", (char *[]){NULL});

        printf("Error al ejecutar el execv desde analisisImage.\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        int cflag, nflag, bflag, totalData, totalSize, i, resultado;
        unsigned long long width, height;
        unsigned int* binaryData; 

        printf("                # Inicio analisisImage => pid(%i) \n", getpid());

        printf("antes leeer\n");
        /* Leyendo datos enviados desde binaryImage */
        read(STDIN_FILENO, &cflag, sizeof(cflag));
        printf("                cflag:%i\n", cflag);
        read(STDIN_FILENO, &nflag, sizeof(nflag));
        printf("                nflag:%i\n", nflag);
        read(STDIN_FILENO, &bflag, sizeof(bflag));
        printf("                bflag:%i\n", bflag);
        read(STDIN_FILENO, &width, sizeof(width));
        printf("                width:%llu\n", width);
        read(STDIN_FILENO, &height, sizeof(height));
        printf("                height:%llu\n", height);

        totalSize = width * height;
        binaryData = (unsigned int*)malloc(sizeof(unsigned int) * totalSize);
        totalData = totalSize * 3;

        for(i = 0; i < totalSize; i++)
        {
            read(STDIN_FILENO, &binaryData[i], sizeof(int));
        }

        /* Hago el analisis */
        resultado = isNearlyBlack(binaryData, nflag, width, height);

        close(pipefd[READ]);
        write(pipefd[WRITE], &cflag, sizeof(cflag));
        write(pipefd[WRITE], &nflag, sizeof(nflag));
        write(pipefd[WRITE], &bflag, sizeof(bflag));
        write(pipefd[WRITE], &width, sizeof(width));
        write(pipefd[WRITE], &height, sizeof(height));

        wait(&status);
        printf("                # Fin analisisImage\n");
        return 0;
    }
}

int isNearlyBlack(unsigned int *binaryData, int nflag, int width, int height)
{
    int i, totalSize, black=0;
    float value;

    totalSize = width * height;
    for(i=0;i<totalSize;i++)
    {
        if(binaryData[i] == 0)
        {
            black++;
        }
    }

    value = ((float)black/(float)totalSize) * 100; 
    if( value >  nflag)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}