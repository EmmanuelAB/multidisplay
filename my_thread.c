//
// Created by emmanuel on 09/05/19.
//

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <linux/futex.h>
#include <syscall.h>
#include "my_thread.h"
#include <stdatomic.h>




int my_thread_create( void *function, int scheduler ) {
    //for testing
    ready_threads_round_robin->format = "%s";
    ready_threads_lottery->format = "%s";

    // if the scheduler hasn't been initialized yet must
    // create TCB so the caller can be scheduled like any other threads
    if(! scheduler_inialized){
        // save this context (the caller)
        TCB *caller_tcb = malloc(sizeof(TCB));
        caller_tcb->context = malloc(sizeof(ucontext_t));
        caller_tcb->id = serial_id++;
        caller_tcb->name = "Main";

        //if scheduler is lottery, give tickets
        if(scheduler == LOTTERY){
            caller_tcb->tickets = 5;
            // insert the caller context in the list is remains as another thread
            list_add_element(ready_threads_lottery, caller_tcb);

        }else{
            // insert the caller context in the list is remains as another thread
            list_add_element(ready_threads_round_robin, caller_tcb);
        }

        // prepare the scheduler context to run
        makecontext(&scheduler_context, schedule_next_thread, 0);

        // avoid entering this if again
        scheduler_inialized = 1;

        // change to scheduler context to start multiplexing the threads
        swapcontext(caller_tcb->context , &scheduler_context);

    }

    if(scheduler == ROUNDROBIN){

        create_tcb_round_robin(function,"thread1");
        create_tcb_round_robin(function,"thread2");
        create_tcb_round_robin(function,"thread3");
        create_tcb_round_robin(function,"thread4");
        create_tcb_round_robin(function,"thread5");

        list_print(ready_threads_round_robin);

    }else if(scheduler == LOTTERY){

        create_tcb_lottery(function,10,"thread1");
        create_tcb_lottery(function,20,"thread2");
        create_tcb_lottery(function,3,"thread3");
        create_tcb_lottery(function,6,"thread4");
        create_tcb_lottery(function,2,"thread5");

        sort_max_min(ready_threads_lottery);

    }else{
        printf("Not specified scheduler");
    }

    return 0;
    //return new_tcb->id;
}

void create_tcb_round_robin(void *function, char *name){
    // create a context where the new thread will run

    TCB *new_tcb = malloc(sizeof(TCB));
    new_tcb->id = serial_id++;
    new_tcb->name = name;
    new_tcb->context = malloc( sizeof(ucontext_t) );
    new_tcb->scheduler = ROUNDROBIN;

    ucontext_t *new_thread_context = new_tcb->context;
    getcontext(new_thread_context);
    //new_tcb->context->uc_link = end_context; // make the link to the finishing context
    ucontext_init_stack(new_thread_context);
    makecontext(new_thread_context, function, 0);

    // insert the thread context in the scheduler's list
    list_add_element(ready_threads_round_robin, new_tcb);
}

void create_tcb_lottery(void *function,int tickets,char *name){
    // create a context where the new thread will run

    TCB *new_tcb = malloc(sizeof(TCB));
    new_tcb->id = serial_id++;
    new_tcb->name = name;
    new_tcb->tickets = tickets;
    new_tcb->context = malloc( sizeof(ucontext_t) );
    new_tcb->scheduler = LOTTERY;

    ucontext_t *new_thread_context = new_tcb->context;
    getcontext(new_thread_context);
    new_tcb->context->uc_link = end_context; // make the link to the finishing context
    ucontext_init_stack(new_thread_context);
    makecontext(new_thread_context, function, 0);

    // insert the thread context in the scheduler's list
    list_add_element(ready_threads_lottery, new_tcb);
}



int my_mutex_trylock( int *lock){
    int return_value;
    int expected = MUTEX_UNLOCKED_VALUE, new_value = MUTEX_LOCKED_VALUE;

    // if the value at lock is == expected => lock is updated (expected remains untouched)
    // if not => the value at lock is saved to expected
    atomic_compare_exchange_strong(lock, &expected, new_value);

    // if couldn't get the lock wait until it's released
    if (expected == MUTEX_LOCKED_VALUE)
        return_value = MUTEX_DENIED;
    else
        return_value = MUTEX_ACQUIRED;

    return return_value;
}



void my_mutex_lock(int *lock){

    int expected = MUTEX_UNLOCKED_VALUE, new_value = MUTEX_LOCKED_VALUE;

    // if the value at lock is == expected => lock is updated (expected remains untouched)
    // if not => the value at lock is saved to expected
    atomic_compare_exchange_strong(lock, &expected, new_value);

    // if couldn't get the lock wait until it's released
    if (expected == MUTEX_LOCKED_VALUE){
        printf("nope\n");
        //sleep until the lock is released
        syscall(SYS_futex, lock, FUTEX_WAIT, MUTEX_LOCKED_VALUE, NULL, NULL, NULL);
        printf("after call\n");
    }
    else{ printf("mine\n"); }

}



void my_mutex_unlock(int *lock){
    // reset the lock's value
    *lock = MUTEX_UNLOCKED_VALUE;

    // wake waiting threads, if any
    int result = syscall(SYS_futex, lock, FUTEX_WAKE, MUTEX_MAX_THREADS_TO_WAKE, NULL, NULL, NULL);

}



static void ucontext_init_stack(ucontext_t *context){

    context->uc_stack.ss_sp = malloc(STACK_SIZE);
    context->uc_stack.ss_size = STACK_SIZE;
    context->uc_stack.ss_flags = 0; // use no flags

}



TCB *round_robin(){
    // Determine the next
    int next_index = (current_context_index + 1) % ready_threads_round_robin->size;

    // Set the next as the current
    current_context_index = next_index;

    printf("run --> %s\n",list_get_element_at(ready_threads_round_robin, current_context_index)->name );

    return list_get_element_at(ready_threads_round_robin, current_context_index);

}

int get_random(int from, int to){
    int number = rand () % (to + 1) + from;
    return number;
}


int process_winner(){

    int winner = get_random(0,total_tickets);
    int sum = 0, i = 0;


    while(i < ready_threads_round_robin->size -1){

        sum += list_get_element_at(ready_threads_lottery, i)->tickets;

        if(sum > winner){
            break;
        }
        i++;
    }
    return i; //id del proceso ganador
}


TCB *lottery(){
    // Determine the next
    int next_index = process_winner();

    //total_tickets -= list_get_element_at(ready_threads_round_robin, next_index)->tickets;

    printf("run --> %s\n", list_get_element_at(ready_threads_lottery, next_index)->name);

    return list_get_element_at(ready_threads_lottery, next_index);

}



/*void schedule_next_thread(){
    // Determine the next thread context to be loaded
    ucontext_t *context_to_run = determine_next_context();


    //update the current context
    current_context = context_to_run;
    /*TODO: check if we can remove this variable and use only current_tcb instead.
     * I think it would be just to replace usages of current_context to current_tcb->context */
/*
    //update the current TCB
    current_tcb = list_get_element_at(ready_threads, current_context_index);

    // Set the alarm
    ualarm(ALARM_FREQUENCY*TO_MICROSECONDS, 0);

    // Load the context to start running the thread
    setcontext(context_to_run);
}*/

void schedule_next_thread(){
    TCB *new_tcb;
    ucontext_t *context_to_run;

    if(SCHEDULER == ROUNDROBIN){
        // Determine the next thread context to be loaded
        new_tcb = round_robin();
        context_to_run = new_tcb->context;
        //update the current context
        current_context = context_to_run;
        // Set the alarm
        alarm(ALARM_FREQUENCY);

    }
    if(SCHEDULER == LOTTERY){
        // Determine the next thread context to be loaded
        new_tcb = lottery();
        context_to_run = new_tcb->context;
        //update the current context
        current_context = context_to_run;
        // Set the alarm
        alarm(new_tcb->tickets);
    }
    // Load the context to start running the thread
    setcontext(context_to_run);
}



void handle_alarm(int signal_number){
    printf("alarm!\n");
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
    ready_threads_round_robin = list_create();

    ready_threads_lottery = list_create();

    // initialize the scheduler context
//    scheduler_context = malloc(STACK_SIZE);
    getcontext(&scheduler_context);

    ucontext_init_stack(&scheduler_context);

    //initialize the context that will be loaded when every thread finishes
    end_context = malloc(sizeof(ucontext_t));
    ucontext_init_stack(end_context);
    getcontext(end_context);
    makecontext(end_context, end_function, 0);


}



void my_mutex_init(int *lock){
    *lock = MUTEX_INIT_LOCK_VALUE;
}


void end_function(int thread_id){
//    printf("END FUNC\n"); return;

    // Remove the element from the TCB
    int index_to_delete = current_context_index;
    if(index_to_delete != LIST_ELEMENT_NOT_FOUND) {
        list_remove_element_at(ready_threads_round_robin, index_to_delete);
        printf("Thread deleted from list\n");
    } else{
        printf("Failed to delete TCB from list\n");
    }

    // Call the scheduler to make it run the next thread
//    kill(getpid(), SIGALRM); //TODO CHECK if this is the way to do it
    handle_alarm(SIGALRM);
}


void my_thread_chsched(int id){
    TCB *thread;

    thread = list_get_element_at(ready_threads_round_robin, id);

    if(thread == NULL){
        thread = list_get_element_at(ready_threads_lottery, id);

        //Delete thread from lottery list
        list_remove_element_at(ready_threads_lottery,id);

        //remove number tickets thread of the total tickets
        total_tickets-= thread->tickets;

        //remove tickets to the thread.
        thread->tickets = 0;

        //insert thread in round robin list
        list_add_element(ready_threads_round_robin, thread);

        //list_print(ready_threads_round_robin);
        //list_print(ready_threads_lottery);

    }else{
        //delete thread from round robin list
        list_remove_element_at(ready_threads_round_robin,id);

        //give tickets to the thread
        thread->tickets = 12;

        //add tickets to total tickets
        total_tickets+=12;

        //insert thread in lottery list
        list_add_element(ready_threads_lottery, thread);

        //sort list for scheduling
        sort_max_min(ready_threads_lottery);

        //list_print(ready_threads_lottery);
        //list_print(ready_threads_round_robin);
    }
}