#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>


#define READ 0  /* Index of the read end of a pipe */
#define WRITE 1 /* Index of he write end of a pipe*/


/*
 * Descripción: Este archivo realiza la lectura de los comandos ingresados por consola, luego en un ciclo que cubre la cantidad de imagenes
 *              realiza la iniciacion del pipe para escribir los datos que se pasaran al siguiente proceso. Luego se realiza un fork , en el hijo se realiza
 *              la copia del descriptor de entrada del proceso, se cierra el pipe correspondiente y se llama con execv al siguiente proceso 'readImage'.
 *              En el padre se escriben los argumentos de entrada que seran utilizado por los siguientes procesos. Finalmente se espera a que el hijo termine
 *              su ejecución.
 * 
 * Entrada: Este archivo recibe los siguientes argumentos de entrada, debe ser el primero en llamarse al momento de querer correr el programa:
 *          Ejemeplo: './main -c 2 -u 100 -n 50 -b'
 *          
 *          -c : Cantidad de imagenes a leer.
 *          -u : Umbral para convertir a escala de grises.
 *          -n : Umbral de procentaje de pixeles negros en la imagen.
 *          -b : Flag para determinar si se imprime por pantalla si es o no 'nearlyBlack'
 * 
 * Salida: Se envia hacia el siguiente proceso: 
 *          cflag -> entero
 *          uflag -> entero
 *          nflag -> entero
 *          bflag -> entero
 */
int main(int argc, char *argv[])
{
    pid_t pid;
    int status;

    int cflag,uflag,nflag,bflag,x,index;
    extern int optopt,opterr;
    extern char* optarg;
    opterr=0;
    int imgCount = 0;

    while((x = getopt(argc, argv, ":c:u:n:b")) != -1)
    {
        switch(x)
        {
            case 'c':
                sscanf(optarg,"%d", &cflag);
                if(cflag <= 0)
                {
                    printf("La bandera -c no puede tener un valor igual o menor a cero.\n");
                    exit(1);
                }
                break;
            case 'u':
                sscanf(optarg,"%d", &uflag);
                if(uflag < 0 || uflag > 255)
                {
                    printf("La bandera -u no puede tener un valor menor a cero o mayor a 255.\n");
                    exit(1);
                }
                break;
            case 'n':
                sscanf(optarg,"%d", &nflag);
                if(nflag <= 0 || nflag > 100)
                {
                    printf("La bandera -n no puede tener un valor menor o igual a cero o mayor a 100.\n");
                    exit(1);
                }
                break;
            case 'b':
                bflag = 1;
                break;
            case '?':
                if(optopt == 'c')
                    fprintf(stderr, "Opcion -%c requiere un argumento.\n", optopt);
                else if(isprint(optopt))
                    fprintf(stderr, "Opcion desconocida '-%c'.\n",optopt);
                else
                    fprintf(stderr, "Opcion con caracter desconocido. '\\x%x'.\n", optopt);
                return 1;
            default:
                printf("Antes de abortar.\n");
                abort();
        }
    }

    if(bflag == 1)
    {
        printf("| Imagen           | NearlyBlack          |\n");
        printf("-------------------------------------------\n");
    }

    while(cflag > 0)
    {
        int pipefd[2];
        
        if(pipe(pipefd) == -1)
        {
            printf("Error creando pipe en Main.\n");
            exit(EXIT_FAILURE);
        }
        
        pid = fork();
        if(pid == -1)
        {
            printf("Error creando el fork en el Main.\n");
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
            // close(pipefd[READ]);

            execv("./readImage", (char *[]){NULL});

            printf("Error al ejecutar el execv desde Main.\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            /* Proceso padre */
            int dupStatus;
            imgCount++;

            close(pipefd[READ]);
            write(pipefd[WRITE], &imgCount, sizeof(int));
            write(pipefd[WRITE], &uflag, sizeof(int));
            write(pipefd[WRITE], &nflag, sizeof(int));
            write(pipefd[WRITE], &bflag, sizeof(int));

            wait(&pid);
            cflag--;
        }
    }

    return 0;
}