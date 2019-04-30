#include <stdio.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t mutex;
pthread_t threads[2];


int thread_func( int thread_id ){

    for (int i = 0 ; i < 3 ; ++i) {

        pthread_mutex_lock(&mutex);
        for (int j = 0 ; j < 5 ; ++j) {
            printf("[%d] Counter: %d\n", thread_id, counter);
            counter++;

        }
        pthread_mutex_unlock(&mutex);


    }

    return 0;
}


int main() {



    pthread_create( &threads[0],  NULL, thread_func, 1);
    pthread_create( &threads[1],  NULL, thread_func, 2);


    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    printf("End main\n");

    return 0;
}