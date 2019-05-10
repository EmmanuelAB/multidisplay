#include <unistd.h>
#include "my_thread.h"


void function2(){
    int n = 1;
    while(1){
//        printf("From thread 2 [%d]\n", n++);
        printf("From thread 2\n");
        usleep(0.25 * TO_MICROSECONDS);
    }
}

void function(){
    int n = 1;
    while(1){
//        printf("From thread 1 [%d]\n", n++);
        printf("From thread 1\n");
        usleep(0.25 * TO_MICROSECONDS);
    }

}

void function3(){
    int n = 1;
    while(1){
//        printf("From thread 1 [%d]\n", n++);
        printf("From thread 3\n");
        usleep(0.25 * TO_MICROSECONDS);
    }
}

int main() {

    my_thread_init();

    my_thread_create( function );

    my_thread_create( function2 );

    my_thread_create( function3 );

    while(1){
        printf("Main doing nothing\n");
        usleep(0.25*TO_MICROSECONDS);
    }
    
    return 0;

}