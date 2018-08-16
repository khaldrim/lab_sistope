#include <iostream>
#include <unistd.h>
#include "./Pipeline/Pipeline.hpp"

using namespace std;

int main(int argc, char** argv){
    int cflag, uflag, nflag, bflag, arg, pipe;
    cout << "Hello, world!\n";

    while((arg = getopt(argc, argv, ":c:u:n:b")) != -1){
        switch(arg){
            case 'c':
                sscanf(optarg, "%d", &cflag);
                break;
            case 'u':
                sscanf(optarg, "%d", &uflag);
                break;
            case 'n':
                sscanf(optarg, "%d", &nflag);
                break;
            case 'b':
                bflag = 1;
                break;
            case '?':
                if(optopt == 'c')
                    fprintf(stderr, "Opcion -%c requiere un argumento.\n", optopt);
                else if(isprint(optopt))
                    fprintf(stderr, "Opcion desconocida '-%c.\n", optopt);
                else
                    fprintf(stderr, "Opcion con caracter desconocido. '\\x%x'.\n", optopt);
                return 1;
            default:
                printf("Antes de abortar.\n");
                abort();
        }
    }

    Pipeline p;

    p.setImgCount(cflag);
    pipe = p.start();

    if(pipe == 0){
        cout << "Pipeline finalizado correctamente.\n";
    } else {
        cout << "Pipeline finalizado incorrectamente.\n";
    }

    // printf("cflag=%d,uflag=%d, nflag=%d, bflag=%d \n", cflag,uflag, nflag, bflag);
    return 0;
}