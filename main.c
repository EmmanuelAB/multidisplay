#include <unistd.h>
#include "my_thread/my_thread.h"

int global_var = 0;
int mutex;

void function2(){
    int n = 0;
    while(n < 10){
        printf("\nFrom thread 2\n");
        usleep(0.25 * TO_MICROSECONDS);
        n++;
    }
    printf("\nTHREAD 2 FINISHED\n");
    my_thread_end();
}

void function(){
    int n = 0;
    while(n < 5){
        printf("\nFrom thread 1\n");
        usleep(0.25 * TO_MICROSECONDS);
        n++;
    }
    printf("\nTHREAD 1 FINISHED\n");
    my_thread_end();

}

void function3(){
    int n = 0;
    while(n < 25){
        printf("\nFrom thread 3\n");
        my_mutex_lock(&mutex);
        global_var++;
        printf("\nGlobal var thread 3 %d\n", global_var);
        my_mutex_unlock(&mutex);
        usleep(0.25 * TO_MICROSECONDS);
        n++;
    }
    printf("\nTHREAD 3 FINISHED\n");
    my_thread_end();
}

void test_function(){
    my_mutex_lock(&mutex);
    printf("\nLocked by thread\n");
}

void test(){
    my_mutex_lock(&mutex);
    printf("\nLocked by main\n");
    my_thread_create(test_function, (int) NULL, ROUNDROBIN );
    while(1){}
}

int main() {

    my_thread_init();
    my_mutex_init(&mutex);


    /*int thread_id1 = my_thread_create(function, (int) NULL, ROUNDROBIN );

    int thread_id2 = my_thread_create(function2, (int) NULL, ROUNDROBIN );

    int thread_id3 = my_thread_create(function3, (int) NULL, ROUNDROBIN );

    int n = 0;
    while(n < 5){
        if(global_var == 3) my_thread_yield();
        my_mutex_lock(&mutex);
        global_var++;
        printf("\nGlobal var main %d\n", global_var);
        my_mutex_unlock(&mutex);
        printf("\nMain doing nothing\n");
        usleep(0.25*TO_MICROSECONDS);
        n++;
    }

    printf("\nFINISHED MAIN\n");

    my_thread_join(thread_id1);
    my_thread_join(thread_id2);
    my_thread_join(thread_id3);

    printf("\nAfter joins\n\n");

    if(SCHEDULER == ROUNDROBIN)
        list_print(ready_threads_round_robin);
    else list_print(ready_threads_lottery);

    printf("\nFinish main\n");*/

    test();

    return 0;

}