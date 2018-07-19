#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include "struct.h"
#include "function.h"

#define _POSIX_BARRIERS 
int lock_read = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;

void *threadMain(void *input) 
{
    INPUTDATA *inputData = (INPUTDATA*)input;

    pthread_mutex_lock(&lock);
    if(lock_read == 0) {
        // Leo la imagen con la primera hebra que llega
        
        
        lock_read++;
    }
    pthread_mutex_unlock(&lock);


    return NULL;
}


int mainMenu(int cflag, int hflag, int uflag, int nflag, int bflag)
{   
    INPUTDATA *inputData = (INPUTDATA*) malloc(sizeof(INPUTDATA));
    
    inputData->cflag = cflag;
    inputData->hflag = hflag;
    inputData->uflag = uflag;
    inputData->nflag = nflag;
    inputData->bflag = bflag;

    pthread_t threadGroup[hflag];
    
    // printf("cflag=%d, hflag=%d,uflag=%d, nflag=%d, bflag=%d \n", inputData->cflag, inputData->hflag,inputData->uflag,inputData->nflag,inputData->bflag);

    int k;
    for(k = 0; k < hflag; k++) {
        if(pthread_create(&threadGroup[k], NULL, threadMain, inputData)) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    }

    pthread_mutex_destroy(&lock);

    for(k = 0; k < hflag; k++) {
        if(pthread_join(threadGroup[k], NULL)) {
            fprintf(stderr, "Error joining thread\n");
            return 2;
        }
    }

    return 0;
}


