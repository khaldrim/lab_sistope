#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "function.h"

int main(int argc, char** argv)
{
    /*
        Variables ingresadas por consola.
            c -> Cantidad de imagenes.
            u -> Umbral para binarizar la imagen.
            n -> Umbral para clasificacion.
            b -> Indica si se debe mostrar los resultados por pantalla al leer la imagen binarizada.
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
                
                /*
                Anañir validaciones de parametros.

                if(cflag <= 0)
                {
                    printf("(!) Valor de C menor o igual a 0.");
                }
                else
                {
                    printf("Valor de C: %d",c_value);
                }
                */
                break;
            case 'u':
                sscanf(optarg,"%d", &uflag);
                break;
            case 'n':
                sscanf(optarg,"%d", &nflag);
                break;
            case 'b':
                /* 
                    bflag activo
                    validar que no debe traer un argumento
                */
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

    printf("cflag=%d, uflag=%d, nflag=%d, bflag=%d \n", cflag, uflag, nflag, bflag);
    mainMenu(cflag, uflag, nflag, bflag);

    return 0;
}