#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

/*
 * Descripcion: Recibe los datos del proceso anterior por la entrada estandar, primero los lee y asigna. Luego lee dato por dato enviado, de la matriz de pixeles, 
 *              y los asigna segun el orden que es Blue, Green, Red, luego viene el dato alpha se realiza la operacion de convertir a escala de grises, y el resultado
 *              se almacena en un arreglo. Luego se escribe por el pipe para enviar los datos al siguiente proceso.
 * 
 * Entrada: Por argumento nada, por la entrada estandar: cflag, uflag, nflag, bflag, width, height, offset, datos de los pixeles.
 * 
 * Salida: Por el pipe: cflag, uflag, nflag, bflag, width, height, offset, datos de los pixeles en escala de grises. 
*/
int main(int argc, char* argv[])
{   
    pid_t pid;
    int pipefd[2];
    int status;

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
        int dupStatus;

        close(pipefd[WRITE]);
        dupStatus = dup2(pipefd[READ], STDIN_FILENO);
        if(dupStatus == -1)
        {
            perror("Dup2 Error: ");
            exit(EXIT_FAILURE);
        }
        close(pipefd[READ]);
        
        execv("./binaryImage", (char *[]){NULL});

        printf("Error al ejecutar el execv desde readImage.\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        /* Proceso padre */
        int cflag, uflag, nflag, bflag, i, j, totalSize, totalData;
        unsigned long long  width, height;
        unsigned char red, green, blue;
        unsigned int offbits;
        double scale;
        unsigned int* scaleData;
                
        read(STDIN_FILENO, &cflag, sizeof(int));
        read(STDIN_FILENO, &uflag, sizeof(int));
        read(STDIN_FILENO, &nflag, sizeof(int));
        read(STDIN_FILENO, &bflag, sizeof(int));
        read(STDIN_FILENO, &width, sizeof(unsigned long long));
        read(STDIN_FILENO, &height, sizeof(unsigned long long));
        read(STDIN_FILENO, &offbits, sizeof(unsigned int));

        totalSize = (int)width * (int)height;
        scaleData = (unsigned int*)malloc(sizeof(unsigned int) * totalSize);
        totalData = totalSize * 4;

        j = 0;
        int k = 0;
        for(i = 0; i < totalData; i++)
        {
            switch(k)
            {
                case 0:
                    read(STDIN_FILENO, &blue, sizeof(unsigned char));
                    k++;
                    break;
                case 1:
                    read(STDIN_FILENO, &green, sizeof(unsigned char));
                    k++;
                    break;
                case 2:
                    read(STDIN_FILENO, &red, sizeof(unsigned char));
                    k++;
                    break;
                case 3:
                    k = 0;
                    scale = (int)red*0.3 + (int)green*0.59 + (int)blue*0.11;
                    scaleData[j] = (int)scale;
                    j++;           
                    break;
            }
  
        }

        close(pipefd[READ]);
        write(pipefd[WRITE], &cflag, sizeof(int));
        write(pipefd[WRITE], &uflag, sizeof(int));
        write(pipefd[WRITE], &nflag, sizeof(int));
        write(pipefd[WRITE], &bflag, sizeof(int));
        write(pipefd[WRITE], &width, sizeof(unsigned long long));
        write(pipefd[WRITE], &height, sizeof(unsigned long long));
        write(pipefd[WRITE], &offbits, sizeof(unsigned int));

        j = 0;
        for(j=0;j<totalSize;j++)
        {
            write(pipefd[WRITE], &scaleData[j], sizeof(unsigned int));
        }

        wait(&pid);
        return 0;
    }
}