#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <sys/time.h>
#include "rand.h"

#define ROUNDS 10 
#define LOOP 100000
#define BUFFER 100

int main() { 
    double timer;
    struct timeval tstart, tend;

    gettimeofday(&tstart, NULL);
    void *init = sbrk(0); 
    void *current ; 

    void* buffer[BUFFER];

    for(int i = 0; i < BUFFER; i++){
        buffer[i] = NULL;
    } 

    printf ("The initial top of the heap is %p.\n", init ); 

    for(int j = 0; j < ROUNDS; j++) { 
        for(int i= 0; i < LOOP ; i++) { 
            int index = rand() % BUFFER;
            if(buffer[index] != NULL){
                free(buffer[index]);
                buffer[index] = NULL;
            }
            else{
                size_t size = request();
                int* memory = malloc(size);
                buffer[index] = memory;
                *memory = 123;
            } 
        } 
        current = sbrk (0); 
        int allocated = (int)(( current - init )); 
        printf ("The current top of the heap is %p.\n", current ); 
        printf ("increased by %d byte\n", allocated ); 
    } 

    gettimeofday(&tend, NULL);
    timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
    printf("Time is %g\n", timer);

    return 0; 
}
