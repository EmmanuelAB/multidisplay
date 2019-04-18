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
#define NUM_OF_THREADS 2


// Where context of each thread will be saved
ucontext_t threads_contexts[NUM_OF_THREADS];

// Sheduler stuff
ucontext_t scheduler_context;
void *scheduler_stack;

// To track which thread is running
int current_context_index = -1; // JUST TO MAKE IT 0 THE FIRST TIME



void thread_function(int id){
    for (int i = 0; i < 1000; ++i) {
        printf("thread %d: %d\n", id, i);
        usleep(0.1 * TO_MICROSECONDS);
    }
    printf("thread finished\n");
}



void create_threads_contexts(ucontext_t contexts[]){
    for(int i=0 ; i < NUM_OF_THREADS ; i++){
        ucontext_t *current_pointer = &contexts[i];

        // Save this context in the struct, it will be modified
        getcontext(current_pointer);

        // Create and set the stack for the thread context
        void *stack = malloc(STACK_SIZE);
        current_pointer->uc_stack.ss_sp =  stack;
        current_pointer->uc_stack.ss_flags = 0; // use no flags
        current_pointer->uc_stack.ss_size = STACK_SIZE;

        // Set the signal mask of the context
        sigemptyset(&current_pointer->uc_sigmask); //TODO check why do this?

        // Modify the context to execute threadfunction(i) when loaded
        makecontext(current_pointer, thread_function, 1, i);

    }
}



ucontext_t *determine_next_context(){
    // Determine the next
    int next_index = current_context_index + 1 % NUM_OF_THREADS;

    // Set the next as the current
    current_context_index = next_index;

    return &threads_contexts[next_index];

}



void schedule_next_thread(){
    // Determine the next thread context to be loaded
    ucontext_t *context_to_run = determine_next_context();

    // Set the alarm
    alarm(ALARM_FREQUENCY);

    // Load the context to start running the thread
    setcontext(context_to_run);
}



void handle_alarm(int signal_number){
    printf("thread was interrupted\n");

    // Create a context to run the scheduler
    getcontext(&scheduler_context);
    (&scheduler_context)->uc_stack.ss_sp =  scheduler_stack;
    (&scheduler_context)->uc_stack.ss_flags = 0; // use no flags
    (&scheduler_context)->uc_stack.ss_size = STACK_SIZE;
    makecontext(&scheduler_context, schedule_next_thread, 0);



    /*
       This is a tricky part
       1) Saves THIS context at the array of contexts so it can be resumed later
       2) Executes the scheduler context which will load another context
       -> MUST BE DONE THIS WAY because if first store the current context and then loads the
       scheduler, when the saved thread runs again it will run immediately the scheduler because it'd
       be the next instruction after the point its context was saved. Therefore doing it this way, when the
       thread is resumed, it returns back to its execution immediately
    */
    swapcontext(&threads_contexts[current_context_index], &scheduler_context);


}



void setup_alarm_handler() {
    struct sigaction action;
    action.sa_handler = handle_alarm;
    sigemptyset (&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGALRM, &action, NULL); // NULL is the old action, not used here
}



void initialize_scheduler(){
    scheduler_stack = malloc(STACK_SIZE);
}



int main() {

    // Establish what to do when the alarm activates
    setup_alarm_handler();


    // Initialize the context where the thread will run at
    create_threads_contexts(threads_contexts);

    // By now it only allocates the stack pointer for the scheduler context
    initialize_scheduler();


    // Start scheduling threads
    schedule_next_thread();


    return 0;

}