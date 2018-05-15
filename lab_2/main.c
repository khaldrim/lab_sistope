#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "struct.h"

#define READ 0  /* Index of the read end of a pipe */
#define WRITE 1 /* Index of he write end of a pipe*/

int main(int argc,char** argv)
{
    /* Pid y Pipe */
    pid_t pid;
    int status;
    int i=0;
    int fdCount[2], fdUflag[2], fdNflag[2];
    
    if( (pipe(fdCount) == -1) || (pipe(fdUflag) == -1) || (pipe(fdNflag) == -1))
    {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if(pid == -1)
    {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0) /* Proceso hijo */
    {
        int count, u, n;
        char buffCount[20];
        char buffUflag[20];
        char buffNflag[20];

        // char **argv = {NULL};
        snprintf(buffCount,2, fdCount);



        execv("./readImage",(char *[]){ buffCount, NULL});

        printf("Execv se ejecuto mal desde el proceso Main.\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        /* Obtengo los parametros de entrada */
        int imgCount = 0;
        int cflag,uflag,nflag,bflag,x,index;
        extern int optopt,opterr;
        extern char* optarg;
        opterr=0;

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

        while(cflag > 0)
        {
            imgCount++;
            close(fdCount[READ]);
            write(fdCount[WRITE], &imgCount, sizeof(imgCount));
            cflag--;
            waitpid(pid, &status,WUNTRACED);
        }
    }
}