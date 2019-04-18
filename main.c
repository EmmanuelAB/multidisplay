#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <ncurses.h>
#include <ucontext.h>
#include <sched.h>

int counting_function(void *row_to_write){
    int actual_row_to_write = *(int*)row_to_write;
    fprintf(stderr,"child writing at row %d \n",actual_row_to_write);
    char *message = "C:";
    move(actual_row_to_write, 0);
    printw("%s",message);

//    fprintf(stderr,"child wrote msg\n");
//    refresh(); sleep(3); return 0;//tmp

    int offset = strlen(message);


    for (int i = 0; i < 10; ++i) {
        move(actual_row_to_write, offset);
        printw("%d", i);
        refresh();
        sleep(1);

    }
    return 0;
}

int  launch_counter(int *row_to_write, void *stack, int stack_size){
//    int flags = SIGCHLD;
    int flags = SIGCHLD | CLONE_VM | CLONE_FILES | CLONE_IO; // use this??
    int thread_id = clone(counting_function, stack+stack_size, flags , row_to_write);
    fprintf(stderr,"Created thread id:%d writing at row %d \n", thread_id, *row_to_write);
    return thread_id;
}



int main() {
    setbuf(stdout, NULL);

    ucontext_t context;
    int x = 10;



    getcontext(&context);
    if(x == 10){
        printf("After saving x=10\n");
        x = 5;
        setcontext(&context);
    }
    else{
        printf("After restoring x = 5\n");
    }

    printf("X is %d\n",x);

    return 0;































//    setbuf(stdout, NULL);
//    scanf("\n");
//    initscr();
//    int stack_size = 1024*1024;
//
//    int counters = 2, threads_rows[counters], threads_ids[counters];
//    void *stacks[counters];
//    for (int i = 0; i < counters; ++i) {
//        threads_rows[i] = i;
//        void *stack = malloc(stack_size);
//        stacks[i] = stack;
//        threads_ids[i] = launch_counter(&threads_rows[i], stack, stack_size);
//    }
//
//    // wait childs to terminate
//    while(counters--){
//        wait(NULL);
//    }
//    // free thread's stacks
//    for (int i = 0; i < counters ; ++i) {
//        free(stacks[i]);
//    }
//
//    endwin();
//    return 0;
}