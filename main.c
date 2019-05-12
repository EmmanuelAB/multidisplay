#include <unistd.h>
#include "my_thread.h"

#define MAIN_CONTEXT_INDEX 0

void function2(){
    int n = 0;
    while(n < 10){
        printf("From thread 2\n");
        usleep(0.25 * TO_MICROSECONDS);
        n++;
    }
    printf("\nTHREAD 2 FINISHED\n");
    my_thread_end();
}

void function(){
    int n = 0;
    while(n < 5){
        printf("From thread 1\n");
        usleep(0.25 * TO_MICROSECONDS);
        n++;
    }
    printf("\nTHREAD 1 FINISHED\n");
    my_thread_end();

}

void function3(){
    int n = 0;
    while(n < 25){
        printf("From thread 3\n");
        usleep(0.25 * TO_MICROSECONDS);
        n++;
    }
    printf("\nTHREAD 3 FINISHED\n");
    my_thread_end();
}

int main() {

    my_thread_init();

    int thread_id1 = my_thread_create( function );

    int thread_id2 = my_thread_create( function2 );

    int thread_id3 = my_thread_create( function3 );

    my_thread_join(thread_id1);
    my_thread_join(thread_id2);
    my_thread_join(thread_id3);

    printf("\nAfter joins\n");
    list_print(ready_threads_list);

    int n = 0;
    while(n < 5){
        printf("Main doing nothing\n");
        usleep(0.25*TO_MICROSECONDS);
        n++;
    }

    //my_thread_join(thread_id1);

    printf("\nFinish main\n");

    return 0;

}