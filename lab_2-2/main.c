#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "struct.h"

#define READ 0  /* Index of the read end of a pipe */
#define WRITE 1 /* Index of he write end of a pipe*/

int main(int argc, char** argv)
{
  /* Obtengo los parametros de entrada */
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

  /* Pid y Pipe */
  pid_t pid;
  int fd[2];

  if(pipe(fd) == -1)
  {
    perror("Error");
    exit(EXIT_FAILURE);
  }
  pid = fork();
  if(pid == -1) /* Error al crear un hijo */
  {
    perror("Error");
    exit(EXIT_FAILURE);
  }
  else if(pid == 0) /* Proceso hijo */
  {
    int uflag = 0;

    close(fd[WRITE]);
    read(fd[READ], &uflag, sizeof(uflag));
    close(fd[READ]);

    printf("Soy el hijo recibi: %i y termie po choro.\n",uflag);
    exit(EXIT_SUCCESS);
  }
  else /* Proceso padre */
  {
    printf("cflag es: %i\n",cflag);
    while(cflag > 0)
    {
      close(fd[READ]);
      write(fd[WRITE], &uflag, sizeof(uflag));
      close(fd[WRITE]);

      wait(NULL);
      cflag--;
    }
  }

  printf("\n# Fin Main #\n");
  return 0;
}
