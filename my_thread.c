//
// Created by emmanuel on 09/05/19.
//

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "my_thread.h"


int my_thread_create( void *function ) {





    // if the scheduler hasn't been initialized yet must
    //
    if(! scheduler_inialized){
        // save this context (the caller)
        TCB *caller_tcb = malloc(sizeof(TCB));
        caller_tcb->context = malloc(sizeof(ucontext_t));
        caller_tcb->id = serial_id++;
        // insert the caller context in the list is remains as another thread
        list_add_element(ready_threads_list, caller_tcb);

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
    makecontext(new_thread_context, function, 0);

    // insert the thread context in the scheduler's list
    list_add_element(ready_threads_list, new_tcb);

    return new_tcb->id;
}




static void ucontext_init_stack(ucontext_t *context){

    context->uc_stack.ss_sp = malloc(STACK_SIZE);
    context->uc_stack.ss_size = STACK_SIZE;
    context->uc_stack.ss_flags = 0; // use no flags

}


ucontext_t *determine_next_context(){
    // Determine the next
    if(ready_threads_list->size == 0){
        exit(0);
    }

    int next_index = (current_context_index + 1) % ready_threads_list->size;

    //printf("\nReady list size %d\n", ready_threads_list->size);

    // Set the next as the current
    current_context_index = next_index;

    //printf("\nCurrent context index %d\n", current_context_index);

    TCB* next_TCB = list_get_element_at(ready_threads_list, current_context_index);

    printf("\nNext context %d\n", next_TCB->id);

    return next_TCB->context;

}


void schedule_next_thread(){
    // Determine the next thread context to be loaded
    ucontext_t *context_to_run = determine_next_context();

    if(context_to_run != NULL) printf("\nContext to run not null\n");
    //update the current context
    current_context = context_to_run;

    // Set the alarm
    alarm(ALARM_FREQUENCY);

    printf("\nBefore set context\n");

    // Load the context to start running the thread
    setcontext(context_to_run);
}


void handle_alarm(int signal_number){
//    printf("thread was interrupted\n");
    printf("Interrupt! || [%d] threads running\n", ready_threads_list->size);

    // Create a context to run the scheduler
//    getcontext(&scheduler_context);
//    (&scheduler_context)->uc_stack.ss_sp =  scheduler_stack;
//    (&scheduler_context)->uc_stack.ss_flags = 0; // use no flags
//    (&scheduler_context)->uc_stack.ss_size = STACK_SIZE;
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

int get_thread_index_waiting_for(TCB* thread1){
    node* iterator = blocked_threads_list->first;
    int index = 0;
    while(iterator != NULL){
        if(iterator->value->waiting_thread_id == thread1->id){
            return index;
        }
        iterator = iterator->next;
        index++;
    }
    return FALSE;
}

void my_thread_end(){

    printf("\nIn END\n");

    // if other thread is waiting for this one push it into the ready list
    int waiting_thread_index = get_thread_index_waiting_for((list_get_element_at(ready_threads_list, current_context_index)));
    //printf("\nWaiting thread index %d\n", waiting_thread_index);

    // remove context from ready list
    list_remove_element_at(ready_threads_list, current_context_index);

    if(waiting_thread_index != FALSE){

        printf("\nSomeone is waiting for me\n");

        TCB* waiting_thread_TCB = list_remove_element_at(blocked_threads_list, waiting_thread_index);

        //printf("\nThread removed from blocked thread list\n");

        printf("\nThread removed %d\n", waiting_thread_TCB->id);

        //printf("\nReady size before %d\n", ready_threads_list->size);

        list_add_element(ready_threads_list, waiting_thread_TCB);

        //printf("\nReady size after %d\n", ready_threads_list->size);

        //printf("\nThread added to ready list\n");

        list_print(ready_threads_list);
        list_print(blocked_threads_list);
    }

    // swap to scheduler context
    setcontext(&scheduler_context);
}


void my_thread_join(int thread_id){

    printf("\nIn join");

    // set waiting id attribute to caller thread
    TCB* caller_thread_TCB = list_get_element_at(ready_threads_list, current_context_index);
    caller_thread_TCB->waiting_thread_id = thread_id;

    //printf("\nCurrent thread index %d\n", current_context_index);
    //printf("Caller id %d\n", caller_thread_TCB->id);
    printf("\nWaiting thread id %d\n", caller_thread_TCB->waiting_thread_id);

    // remove caller thread from scheduler ready list
    TCB* caller_threadTCB = list_remove_element_at(ready_threads_list, current_context_index);
    // push caller thread to waiting list
    list_add_element(blocked_threads_list, caller_threadTCB);

    list_print(ready_threads_list);
    list_print(blocked_threads_list);

    // swap to scheduler context
    swapcontext(caller_thread_TCB->context, &scheduler_context);
}

void my_thread_yield(){
    // raise the signal to activate the handle alarm and swap to
    // scheduler context after saving current
    kill(getpid(), SIGALRM);
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
    ready_threads_list = list_create();
    blocked_threads_list = list_create();

    // initialize the scheduler context
//    scheduler_context = malloc(STACK_SIZE);
    getcontext(&scheduler_context);

    ucontext_init_stack(&scheduler_context);

}