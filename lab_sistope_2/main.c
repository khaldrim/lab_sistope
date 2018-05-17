#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>


#define READ 0  /* Index of the read end of a pipe */
#define WRITE 1 /* Index of he write end of a pipe*/

int main(int argc, char *argv[])
{
    pid_t pid;
    int status = 0;

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
            dupStatus = dup2(pipefd[READ], STDOUT_FILENO);
            if(dupStatus == -1)
            {
                perror("Dup2 Error: ");
                exit(EXIT_FAILURE);
            }

            close(pipefd[READ]);
            
            execv("./readImage", (char *[]){NULL});

            printf("Error al ejecutar el execv desde Main.\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            /* Proceso padre */
            int dupStatus;
            imgCount++;
            cflag--;
            
            printf("\n# Inicio Main => pid(%i) \n", getpid());
            close(pipefd[READ]);

            write(pipefd[WRITE], &imgCount, sizeof(imgCount));
            write(pipefd[WRITE], &uflag, sizeof(uflag));
            write(pipefd[WRITE], &nflag, sizeof(nflag));

            close(pipefd[WRITE]);

            wait(&status);
            printf("# Fin ciclo: %i \n", cflag+1);            
        }
    }

    printf("# Fin Main.\n");
    return 0;
}