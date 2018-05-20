#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

int isNearlyBlack(unsigned int *binaryData, int nflag, int width, int height);

/*
 * Descripcion: Recibe los datos del proceso anterior por la entrada estandar, primero los lee y asigna. Luego lee dato por dato binarizado, luego estos datos se
 *              envian a la funcion 'isNearlyBlack'. Luego se imprime por pantalla si bflag es 1. Luego se escribe en el pipe para enviarlos al siguiente proceso.
 * 
 * Entrada: Por argumento nada, por la entrada estandar: cflag, nflag, bflag, width, height, offset, datos de los pixeles binarizados.
 * 
 * Salida: Por el pipe: cflag, width, height, offset, datos de los pixeles binarizados. 
*/
int main(int argc, char* argv[])
{
    pid_t pid;
    int pipefd[2];

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
        
        close(pipefd[WRITE]);
        dupStatus = dup2(pipefd[READ], STDIN_FILENO);
        if(dupStatus == -1)
        {
            perror("Dup2 Error: ");
            exit(EXIT_FAILURE);
        }

        execv("./writeImage", (char *[]){NULL});

        printf("Error al ejecutar el execv desde analisisImage.\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        int cflag, nflag, bflag, totalSize, i, resultado;
        unsigned long long width, height;
        unsigned int offbits;
        unsigned int* binaryData; 

        read(STDIN_FILENO, &cflag, sizeof(int));
        read(STDIN_FILENO, &nflag, sizeof(int));
        read(STDIN_FILENO, &bflag, sizeof(int));
        read(STDIN_FILENO, &width, sizeof(unsigned long long));
        read(STDIN_FILENO, &height, sizeof(unsigned long long));
        read(STDIN_FILENO, &offbits, sizeof(unsigned int));

        totalSize = width * height;
        binaryData = (unsigned int*)malloc(sizeof(unsigned int) * totalSize);

        for(i = 0; i < totalSize; i++)
        {
            read(STDIN_FILENO, &binaryData[i], sizeof(unsigned int));
        }

        resultado = isNearlyBlack(binaryData, nflag, width, height);
        if(bflag == 1)
        {
            if(resultado == 1)
            {
                printf("| imagen_%i         | Yes                  |\n", cflag);
            }
            else
            {
                printf("| imagen_%i         | No                   |\n", cflag);
            }
        }
        
        close(pipefd[READ]);
        write(pipefd[WRITE], &cflag, sizeof(int));
        write(pipefd[WRITE], &width, sizeof(unsigned long long));
        write(pipefd[WRITE], &height, sizeof(unsigned long long));
        write(pipefd[WRITE], &offbits, sizeof(unsigned int));

        for(i=0;i<totalSize;i++)
            write(pipefd[WRITE], &binaryData[i], sizeof(unsigned int));

        wait(&pid);
        return 0;
    }
}

/*
 * Descripcion: Funcion que recibe los datos binarizados y el ciclo cuenta la cantidad de 0 en el arreglo
 *              (Recordar que ese 0 representa que un pixel se escalo a negro). Luego realiza una division
 *              para calcuar el porcentaje de cuantos pixeles negros posee la imagen, asi se compara si la 
 *              imagen tiene una mayor cantidad de pixeles negros comparados con el umbral ingresado en 'nflag'.
 *              Se retorna un 1 si se decide que es 'nearlyblack', sino se retorna un 0.
 * 
 * Entrada: Puntero arreglo de enteros 'binaryData', Entero parametro 'nflag', Entero ancho 'width', Entero largo 'height'.
 * 
 * Salida: Entero.
 */
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