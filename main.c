//#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <ncurses.h>
#include <ucontext.h>
#include <sched.h>

#define TO_MICROSECONDS 1000000
#define STACK_SIZE 1024*1024
#define ALARM_FREQUENCY 1



ucontext_t thread_context;


void thread_function(){
    for (int i = 0; i < 1000; ++i) {
        printf("%d\n", i);
        usleep(0.1 * TO_MICROSECONDS);
    }
    printf("thread finished\n");
}

void handle_alarm(int signal_number){
    printf("thread was interrupted\n");
    alarm(ALARM_FREQUENCY);
}


void setup_alarm_handler() {
    struct sigaction action;
    action.sa_handler = handle_alarm;
    sigemptyset (&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGALRM, &action, NULL); // NULL is the old action, not used here
}

void create_thread_context(ucontext_t *context){
    // Save this context in the struct
    getcontext(context);

    // Create and set the stack for the thread context
    void *stack = malloc(STACK_SIZE);
    context->uc_stack.ss_sp =  stack;
    context->uc_stack.ss_flags = 0; // use no flags
    context->uc_stack.ss_size = STACK_SIZE;

    // Set the signal mask of the context
    sigemptyset(&context->uc_sigmask);

    // Modify the context to execute threadfunction() when loaded
    makecontext(context, thread_function, 1);
}

int main() {

    // Establish what to do when the alarm activates
    setup_alarm_handler();


    // Initialize the context where the thread will run at
    create_thread_context(&thread_context);


    // Set the alarm to interrupt the thread
    alarm(ALARM_FREQUENCY);


    // start running the thread
    setcontext(&thread_context);


    // do this ever run? no, this never run
    printf("Main finished\n");


    return 0;

}