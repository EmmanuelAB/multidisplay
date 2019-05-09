#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <linux/futex.h>
#include <memory.h>
#include <errno.h>
#include <syscall.h>
#include <stdatomic.h>


#define MUTEX_LOCKED_VALUE 1
#define MUTEX_UNLOCKED_VALUE 0
#define INIT_LOCK_VALUE MUTEX_UNLOCKED_VALUE
#define MAX_THREADS_TO_WAKE 100

int counter = 0;
int var = 0;
pthread_mutex_t mutex;
pthread_t threads[2];
int my_mutex;

void init_lock(int *lock){
    *lock = INIT_LOCK_VALUE;
}


void check_result(int i){
    if (i == -1){
//        printf("Error en syscall: ");
        printf("%s\n",strerror(errno));
    }else{
//        printf("Syscall success!\n");

    }
}


void mylock(int *lock, int tid){

    int expected = MUTEX_UNLOCKED_VALUE, new_value = MUTEX_LOCKED_VALUE;
    
    // if the value at lock is == expected => lock is updated (expected remains untouched)
    // if not => the value at lock is saved to expected
    atomic_compare_exchange_strong(lock, &expected, new_value);

//    printf("After atomic: %d\n", expected);

    // if couldn't get the lock wait until it's released
    if (expected == MUTEX_LOCKED_VALUE){
        printf("[%d] Couldn't get the lock\n",tid);
        
        printf("[%d] Sleeping until it's released\n", tid);
        
        //sleep until the lock is released
        syscall(SYS_futex, lock, FUTEX_WAIT, MUTEX_LOCKED_VALUE, NULL, NULL, NULL);
        printf("[%d] Lock released!\n", tid);

    }else{
        // if got the lock continue normally
        printf("[%d] Got lock!\n", tid);
    }

}


void myunlock(int *lock, int tid){
    // reset the lock's value
    *lock = MUTEX_UNLOCKED_VALUE;

    // wake waiting threads, if any
    int result = syscall(SYS_futex, lock, FUTEX_WAKE, MAX_THREADS_TO_WAKE, NULL, NULL, NULL);

    check_result(result);

    printf("[%d] released the lock\n", tid);

}


void *thread_func( void *pointer){
    int thread_id = *((int*) pointer);

    for (int i = 0 ; i < 1 ; ++i) {

//        pthread_mutex_lock(&mutex);
        mylock(&my_mutex, thread_id);
        for (int j = 0 ; j < 5 ; ++j) {
            printf("[%d] Counter: %d\n", thread_id, counter);
            counter++;

        }
        sleep(2);
        myunlock(&my_mutex, thread_id);
//        pthread_mutex_unlock(&mutex);
        printf("\n");

    }
}

void *suma( void *pointer ){
    int thread_id = *((int*) pointer);
    for (int i = 0; i < 10; i++) {
        mylock(&my_mutex, thread_id);
        //pthread_mutex_lock(&mutex);
        var++;
        printf("\nVAR: %d\n", var);
        //pthread_mutex_unlock(&mutex);
        myunlock(&my_mutex, thread_id);
    }
}


int main() {

    init_lock(&my_mutex);

    int ids[] = {1,2};

    //pthread_create( &threads[1],  NULL, thread_func, &(ids[1]));
    //sleep(1);
    //pthread_create( &threads[0],  NULL, thread_func, &(ids[0]));

    pthread_create(&threads[1], NULL, suma, &(ids[0]));
    pthread_create(&threads[0], NULL, suma, &(ids[1]));
    pthread_join(threads[1], NULL);
    pthread_join(threads[0], NULL);


    printf("End main\n");

    return 0;
}