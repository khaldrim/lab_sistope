#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "function.h"

/*
 * Descripcion: Permite ingresar parametros por consola, los cuales son los siguientes:
 *                c -> Cantidad de imagenes.
 *                u -> Umbral para binarizar la imagen.
 *                n -> Umbral para clasificacion.
 *                b -> Indica si se debe mostrar los resultados por pantalla al leer la imagen binarizada.
 */
int main(int argc, char** argv)
{
    /*
       
    */
    int cflag = 0;
    int uflag = 0;
    int nflag = 0;
    int bflag = 0;

    int x;
    int index;

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

    //printf("cflag=%d, uflag=%d, nflag=%d, bflag=%d \n", cflag, uflag, nflag, bflag);
    mainMenu(cflag, uflag, nflag, bflag);

    return 0;
}