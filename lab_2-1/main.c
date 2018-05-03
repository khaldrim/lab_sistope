#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "struct.h"
#include "main.h"

#define READ 0  /* Index of the read end of a pipe */
#define WRITE 1 /* Index of he write end of a pipe*/

/*
 * Descripcion: Permite ingresar parametros por consola, los cuales son los siguientes:
 *                c -> Cantidad de imagenes.
 *                u -> Umbral para binarizar la imagen.
 *                n -> Umbral para clasificacion.
 *                b -> Indica si se debe mostrar los resultados por pantalla al leer la imagen binarizada.
 */
int main(int argc, char** argv)
{
    int cflag = 0;
    int uflag = 0;
    int nflag = 0;
    int bflag = 0;

    int x, index;

    extern int optopt;
    extern int opterr;
    extern char* optarg;
    opterr = 0;


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

    mainMenu(cflag, uflag, nflag, bflag);
    printf("\n\n ### FIN PROCESO PID: %i ### \n",getpid());
    return 0;
}

void mainMenu(int cflag, int uflag, int nflag, int bflag)
{
    /* Variables */
    int cValue, imgCount, status;
    int* imgPrintResult = NULL;
    char bufferImg[5], bufferUflag[5], bufferNflag[5];

    /* PIDS Y PIPES */
    pid_t pid;
    int pipePC[2]; /* pipe Parent -> Child */

    /* Inicializacion de resultados*/
    imgPrintResult = (int*)malloc(sizeof(int)*cflag);
    if(imgPrintResult == NULL)
    {
        printf("No se logro asignar memoria para imprimir resultados por pantalla.\n");
        exit(1);
    }

    if((pipe(pipePC)) == -1)
    {
        printf("Error creando el pipe de comunicacion del proceso MAIN a readImage.\n");
        exit(EXIT_FAILURE);
    }

    cValue = cflag;
    status = 0;
    imgCount = 0;

    printf("PID PADRE: %i\n", getpid());

    while(cValue > 0)
    {
        printf("\ncValue: %i | imgCount: %i\n",cValue,imgCount);
        pid = fork();
        if(pid == -1) /* Error */
        {
            printf("Error creando el proceso readImage desde el proceso Main.\n");
            exit(EXIT_FAILURE);
        }
        else if(pid == 0) /* Hijo */
        {
            //Leo el valor que me envia el padre
            close(pipePC[WRITE]);
            read(pipePC[READ], &imgCount, sizeof(imgCount));
            close(pipePC[WRITE]);

            //Transformo a char los valores a pasar como parametro
            sprintf(bufferImg,"%d",imgCount);
            sprintf(bufferUflag,"%d",uflag);
            sprintf(bufferNflag,"%d",nflag);
            

            char *argv[4] = {bufferImg, bufferUflag, bufferNflag, NULL}; /* argv to execv*/
            execv("readImage",argv);            
        }
        else /* Padre */
        {
            imgCount++;
            cValue--;
            
            //Paso el valor de imgCount al hijo.
            close(pipePC[READ]); 
            write(pipePC[WRITE], &imgCount, sizeof(imgCount));
            close(pipePC[WRITE]);
            
            //Espero que el hijo termine su ejecucion
            wait(NULL);
        }
    }

    // if(bflag == 1)
    // {
    //     printResult(imgPrintResult, cflag);
    // }
}

