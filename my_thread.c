//
// Created by emmanuel on 09/05/19.
//

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "my_thread.h"


int my_thread_create( void *function, int param ) {





    // if the scheduler hasn't been initialized yet must
    //
    if(! scheduler_inialized){
        // save this context (the caller)
        TCB *caller_tcb = malloc(sizeof(TCB));
        caller_tcb->context = malloc(sizeof(ucontext_t));

        // insert the caller context in the list is remains as another thread
        list_add_element(ready_threads, caller_tcb);

        // prepare the scheduler context to run
        makecontext(&scheduler_context, schedule_next_thread, 0);

        // avoid entering this if again
        scheduler_inialized = 1;

        // change to scheduler context to start multiplexing the threads
        swapcontext(caller_tcb->context , &scheduler_context);

    }

    // create a context where the new thread will run
    TCB *new_tcb = malloc(sizeof(TCB));
    new_tcb->id = serial_id++;
    new_tcb->context = malloc( sizeof(ucontext_t) );
    ucontext_t *new_thread_context = new_tcb->context;
    getcontext(new_thread_context);
    ucontext_init_stack(new_thread_context);
    makecontext(new_thread_context, function, 1, param);

    // insert the thread context in the scheduler's list
    list_add_element(ready_threads, new_tcb);

    return new_tcb->id;
}




static void ucontext_init_stack(ucontext_t *context){

    context->uc_stack.ss_sp = malloc(STACK_SIZE);
    context->uc_stack.ss_size = STACK_SIZE;
    context->uc_stack.ss_flags = 0; // use no flags

}


ucontext_t *determine_next_context(){
    // Determine the next
    int next_index = (current_context_index + 1) % ready_threads->size;

    // Set the next as the current
    current_context_index = next_index;

    return list_get_element_at(ready_threads, current_context_index)->context;

}


void schedule_next_thread(){
    // Determine the next thread context to be loaded
    ucontext_t *context_to_run = determine_next_context();


    //update the current context
    current_context = context_to_run;

    // Set the alarm
    alarm(ALARM_FREQUENCY);

    // Load the context to start running the thread
    setcontext(context_to_run);
}


void handle_alarm(int signal_number){
//    printf("Interrupt! || [%d] threads running\n", ready_threads->size);

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
    ucontext_t *curr = current_context;

    swapcontext(current_context, &scheduler_context);


}



void setup_alarm_handler() {
    struct sigaction action;
    action.sa_handler = handle_alarm;
    sigemptyset (&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGALRM, &action, NULL); // NULL is the old action, not used here
}


void my_thread_init(){

    // set the alarm handler
    setup_alarm_handler();

    // initialize the list of ready threads
    ready_threads = list_create();

    // initialize the scheduler context
//    scheduler_context = malloc(STACK_SIZE);
    getcontext(&scheduler_context);

    ucontext_init_stack(&scheduler_context);

}

