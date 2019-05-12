#include <unistd.h>
#include <ncurses.h>
#include <pthread.h>
#include "my_thread.h"

int mutex;
pthread_mutex_t mut;
int counter = 0;
pthread_t threads[2];


void test_function(){
    int cont= 0;
    while(cont <= 10){
        usleep(0.25 * TO_MICROSECONDS);
        cont++;
    }
}

void function(int row) {
    int n = 1;
    while (n--) {

//        pthread_mutex_lock(&mut);
        my_mutex_lock(&mutex);
//        printf("after lock\n");

        for (int j = 0; j < 5; ++j) {
            printf("[%d] Counter: %d\n", row, counter);
            counter++;
            usleep(0.1*TO_MICROSECONDS);
        }

        printf("\n");
//        usleep(4*TO_MICROSECONDS);

        my_mutex_unlock(&mutex);
//        pthread_mutex_unlock(&mut);

    }
}



void get_lock(int n){
    printf("[%d] started\n", n);
    if(my_mutex_trylock(&mutex) == MUTEX_ACQUIRED){
        printf("[%d] I got the lock\n", n);
    }else{
        printf("[%d] I couldn't get the lock\n", n);
    }
    printf("[%d] ended\n", n);
//    sleep(1);

}

void use_lock(int n){
    printf("[%d] started\n", n);
    printf("[%d] trying to lock\n", n);

    my_mutex_lock(&mutex);

    printf("[%d] got the lock\n", n);
    sleep(1);

    my_mutex_unlock(&mutex);

}


void test_lock(){
    my_thread_init();

    my_mutex_init(&mutex);

    my_thread_create(use_lock, 1);
    printf("[%d] launched\n", 1);

    my_thread_create(use_lock, 2);
    printf("[%d] launched\n", 2);

    int n=1000;
    while(n--){
        printf("main sleeping\n");
        usleep(0.001*TO_MICROSECONDS);
    }
//    printf("main sleeping\n");
//    sleep(6);
//    printf("main finished\n");
}


void use_pthread(){
    pthread_mutex_init(&mut, NULL);
    my_mutex_init(&mutex);

    pthread_create( &threads[1],  NULL, function, 1);
    pthread_create( &threads[1],  NULL, function, 2);
    while(1){}
}




void use_my(){
    my_thread_init();


    my_mutex_init(&mutex);


    my_thread_create( function, 1);
    my_thread_create( function, 2);

    while(1){

    }
}


void test_trylock(){
    my_thread_init();

    my_thread_create(get_lock, 1);
    printf("[%d] launched\n", 1);

    my_thread_create(get_lock, 2);
    printf("[%d] launched\n", 2);

//    while(1){
//        printf("main sleeping\n");
//        usleep(0.001*TO_MICROSECONDS);
//    }
    printf("main sleeping\n");
    while(1);
    printf("main finished\n");

}



int main() {
    srand(time(NULL));
//    use_pthread();
//    use_my();
//    test_trylock();
//    test_lock();

    my_thread_init();

    my_thread_create( test_function , LOTTERY);

    while(1){
        //printf("Main doing nothing\n");
        usleep(0.25*TO_MICROSECONDS);

    }
}
