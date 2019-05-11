#include <unistd.h>
#include <ncurses.h>
#include <pthread.h>
#include "my_thread.h"

int mutex;
pthread_mutex_t mut;
int counter = 0;
pthread_t threads[2];

void function(int row) {
    int n = 1;
    while (n--) {

//        pthread_mutex_lock(&mut);
        my_thread_lock(&mutex);
//        printf("after lock\n");

        for (int j = 0; j < 5; ++j) {
            printf("[%d] Counter: %d\n", row, counter);
            counter++;
            usleep(0.1*TO_MICROSECONDS);
        }

        printf("\n");
//        usleep(4*TO_MICROSECONDS);

        my_thread_unlock(&mutex);
//        pthread_mutex_unlock(&mut);

    }
}


void use_pthread(){
    pthread_mutex_init(&mut, NULL);
    my_thread_mutex_init(&mutex);

    pthread_create( &threads[1],  NULL, function, 1);
    pthread_create( &threads[1],  NULL, function, 2);
    while(1){}
}

void use_my(){
    my_thread_init();


    my_thread_mutex_init(&mutex);


    my_thread_create( function, 1);
    my_thread_create( function, 2);

    while(1){

    }
}

int main() {
//    use_pthread();
    use_my();
}