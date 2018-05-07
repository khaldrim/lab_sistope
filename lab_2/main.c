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
    /* Obtengo los parametros de entrada */
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

    /* Pid y Pipe */
    pid_t pid;
    int imgCount = 0;



    int i=0;
    while(cflag > 0)
    {
        int fd[2];
        
        if(pipe(fd) == -1)
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
            printf("Hijo desde main.\n");
            int count;
            char argCount[10];

            close(fd[WRITE]);
            read(fd[READ],&count,sizeof(count));
            close(fd[READ]);

            // printf("Lei %i desde el hijo %i\n",count,getpid());
            // char **argv = {NULL};
            sprintf(argCount,"%d",count);

            execv("readImage",(char *[]){"./readImage", "-c", argCount,NULL});

            printf("Execv se ejecuto mal desde el proceso Main.\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("i: %i | cflag: %i | imgCount: %i\n",i,cflag,imgCount);

            close(fd[READ]);
            write(fd[WRITE], &imgCount, sizeof(imgCount));
            close(fd[WRITE]);

            wait(NULL);
            cflag--;
            imgCount++;
            i++;
        }
    }
}