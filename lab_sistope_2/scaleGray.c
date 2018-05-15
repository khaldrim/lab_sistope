#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/poll.h>

int main(int argc, char* argv[])
{
    struct pollfd fds[1];
    int pollstatus;
    int cflag, uflag, nflag;
    char buff[9];

    printf("        LLego a scaleGray??\n");

    return 0;

    
    printf("        # Inicio scaleGray => pid(%i) \n", getpid());
    
    read(STDIN_FILENO, &cflag, sizeof(cflag));
    read(STDIN_FILENO, &uflag, sizeof(uflag));
    read(STDIN_FILENO, &nflag, sizeof(nflag));
    

    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN | POLLOUT;

    //                file descrip. , num estructuras, tiempo 
    pollstatus = poll(fds, 1, 5000);
    if(pollstatus > 0)
    {
        if(fds[0].revents & POLLIN)
        {
            read(STDIN_FILENO, &buff, sizeof(buff));
        }
    }

    printf("        cflag: %i | uflag: %i | nflag: %i \n",cflag, uflag, nflag);
    printf("        desde el poll: %s\n", buff);

    printf("        # Fin scaleGray => pid(%i) \n", getpid());
    return 0;
}