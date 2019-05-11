//
// Created by emmanuel on 09/05/19.
//

#ifndef MULTIDISPLAY_MY_THREAD_H
#define MULTIDISPLAY_MY_THREAD_H

#include <ucontext.h>
#include "list.h"


#define ALARM_FREQUENCY 1
#define TO_MICROSECONDS 1000000

#define KB 1000
#define STACK_SIZE 1000 * KB // the stack used by the thread's contexts

static int serial_id = 0; // a consecutive serial to track the current track id to generate and return

static int current_context_index = -1; // just to make it work the first time

static list *ready_threads_list;

static list *blocked_threads_list;

static int scheduler_inialized = 0;

static ucontext_t scheduler_context; // where the context will decide who's next running

static ucontext_t *current_context; // the running context, used when when the alarm occurs



//static void *scheduler_stack;


/*
 * Creates a context to run the received function
 * Insert the context in the scheduler's list
 * Returns the id of the created thread
 * After calling, the caller becomes also a thread
*/
int my_thread_create( void *function );



/*
 * Allocates a stack of -STACK_SIZE- bytes and sets it to the -context-
*/
static void ucontext_init_stack(ucontext_t *context);

// This function checks in the waiting list If some thread is waiting for thread1
// and if so returns its index otherwise returns 0
int get_thread_index_waiting_for(TCB* thread1);


/*
 * Initializes all the struct needed by the library
 */
void my_thread_init();


/*
 * Establishes what to do when the alarm interrupt occurs
 */
void setup_alarm_handler();


/*
 *
 */
void schedule_next_thread();

/*
 *
 */
void my_thread_end();

/*
 *
 */
void my_thread_yield();

/*
 *
 */
void my_thread_join();

/*
 * Based on circular shifting
 */
ucontext_t *determine_next_context();

#endif //MULTIDISPLAY_MY_THREAD_H
