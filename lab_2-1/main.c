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
    return 0;
}

void mainMenu(int cflag, int uflag, int nflag, int bflag)
{
    /* Pid y Pipes */
    pid_t pid;
    int pipeFileHeader[2], pipeInfoHeader[2], pipeData[2];
    
    if((pipe(pipeFileHeader) == -1) || (pipe(pipeInfoHeader) == -1) || (pipe(pipeData) == -1))
    {
        printf("Error iniciado pipes en main.\n");
        exit(EXIT_FAILURE);
    }


    pid = fork();
    if(pid == -1)
    {
        printf("Error al crear hijo de Main -> readImage.\n");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        // Hijo

        /* Variables del hijo */
        BITMAPFILEHEADER *bmpFileHeader = NULL;
        BITMAPINFOHEADER *bmpInfoHeader = NULL;
        DATA *data = NULL;

        char bufferCflag[3];
        char bufferUflag[3];
        char bufferNflag[3];

        char bufferFHPointer[25];
        char bufferIHPointer[25];
        char bufferDPointer[25];

        /* Leer el pipe del padre */
        close(pipeFileHeader[WRITE]);
        close(pipeInfoHeader[WRITE]);
        close(pipeData[WRITE]);

        write(pipeFileHeader[READ],&bmpFileHeader,sizeof(bmpFileHeader));
        write(pipeInfoHeader[READ],&bmpInfoHeader,sizeof(bmpInfoHeader));
        write(pipeData[READ],&data,sizeof(data));

        close(pipeFileHeader[READ]);
        close(pipeInfoHeader[READ]);
        close(pipeData[READ]);

        /* Transformar a char los parametros a pasar */
        sprintf(bufferCflag,"%d",cflag);
        sprintf(bufferUflag,"%d",uflag);
        sprintf(bufferNflag,"%d",nflag);

        sprintf(bufferFHPointer,"%d",&bmpFileHeader);
        sprintf(bufferIHPointer,"%d",&bmpInfoHeader);
        sprintf(bufferDPointer,"%d",&data);

        char *args = {bufferCflag,bufferUflag,bufferNflag,bufferFHPointer,bufferIHPointer,bufferDPointer,NULL};
        execv("readImage",args); 
    }
    else
    {
        // Padre

        /* Variables del padre */
        BITMAPFILEHEADER *bmpFileHeader = NULL;
        BITMAPINFOHEADER *bmpInfoHeader = NULL;
        DATA *data = NULL;


        /* Inicializacion */
        bmpFileHeader = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
        bmpInfoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
        data = (DATA*)malloc(sizeof(DATA));

        bmpInfoHeader->width = 999;

        close(pipeFileHeader[READ]);
        close(pipeInfoHeader[READ]);
        close(pipeData[READ]);

        write(pipeFileHeader[WRITE],&bmpFileHeader,sizeof(bmpFileHeader));
        write(pipeInfoHeader[WRITE],&bmpInfoHeader,sizeof(bmpInfoHeader));
        write(pipeData[WRITE],&data,sizeof(data));

        close(pipeFileHeader[WRITE]);
        close(pipeInfoHeader[WRITE]);
        close(pipeData[WRITE]);

        /* Padre debe esperar a hijo aca */
        wait(NULL);
    }
}
