#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

int main(int argc, char* argv[])
{
    int cflag, nflag, bflag, width, height;

    /* Leyendo datos enviados desde scaleGray */
    read(STDIN_FILENO, &cflag, sizeof(cflag));
    read(STDIN_FILENO, &nflag, sizeof(nflag));
    read(STDIN_FILENO, &bflag, sizeof(bflag));
    read(STDIN_FILENO, &width, sizeof(width));
    read(STDIN_FILENO, &height, sizeof(height));

    printf(" cflag en writeImage es: %i\n", cflag);
    return 0;
}