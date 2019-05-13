//
// Created by emmanuel on 09/05/19.
//

#ifndef MULTIDISPLAY_MY_THREAD_H
#define MULTIDISPLAY_MY_THREAD_H

#include <ucontext.h>
#include "../linked_list/list.h"


#define ALARM_FREQUENCY 0.01
#define TO_MICROSECONDS 1000000

#define MUTEX_LOCKED_VALUE 1
#define MUTEX_UNLOCKED_VALUE 0
#define MUTEX_INIT_LOCK_VALUE MUTEX_UNLOCKED_VALUE
#define MUTEX_MAX_THREADS_TO_WAKE 100
#define MUTEX_ACQUIRED MUTEX_UNLOCKED_VALUE
#define MUTEX_DENIED MUTEX_LOCKED_VALUE


#define KB 1000
#define STACK_SIZE 1000 * KB // the stack used by the thread's contexts

#define ROUNDROBIN 1
#define LOTTERY 2

// 1 for round robin, 2 for lottery, simulation scheduler real time
static int SCHEDULER = ROUNDROBIN;//for testing

static int total_tickets = 45;

static int serial_id = 0; // a consecutive serial to track the current track id to generate and return

static int current_context_index = -1; // just to make it work the first time

static list *ready_threads_round_robin;

static list *ready_threads_lottery;

static list *blocked_threads_list_round_robin;

static list *blocked_threads_list_lottery;

static int scheduler_initialized = 0;

static ucontext_t scheduler_context; // where the context will decide who's next running

static ucontext_t *current_context; // the running context, used when when the alarm occurs

static ucontext_t *end_context;

static TCB *current_tcb;



//static void *scheduler_stack;


/*
 * Creates a context to run the received function
 * Insert the context in the scheduler's list
 * Returns the id of the created thread
 * After calling, the caller becomes also a thread
*/
int my_thread_create( void *function, int param, int scheduler );


/*
 * Creates a TCB that starts at -function- and inserts it into the round robin list
*/
void create_tcb_round_robin(void *function, char *name, int param);

/*
 * Creates a TCB that starts at -function- and inserts it into the lottery list
*/
void create_tcb_lottery(void *function,int tickets,char *name);

/*
 * Allocates a stack of -STACK_SIZE- bytes and sets it to the -context-
*/
static void ucontext_init_stack(ucontext_t *context);



/*
 * Initializes all the struct needed by the library
 */
void my_thread_init();

//Initialize the final context this when a thread ends
void initialize_end_context();


/*
 * Establishes what to do when the alarm interrupt occurs
 */
void setup_alarm_handler();


/*
 *
 */
void schedule_next_thread();


/*
 * Based on circular shifting
 */
TCB *round_robin();

//Return a random number between two numbers
int get_random(int from, int to);

//Return the id of the winner thread
int process_winner();

//Return the winner thread
TCB *lottery();

//change scheduler the one thread from round robin to lottery or reverse
//for change the scheduler of a thread
void my_thread_chsched(int thread_id);


/*
 * Tries to get the lock, if it's free return immediately
 * If it's closed then block until it's freed
 */
void my_mutex_lock( int *lock);




/*
 * Tries to get the lock, if it's free return immediately
 * If it's closed then return an error status
 */
int my_mutex_trylock(int *lock);



/*
 * Sets the value at -lock- to free and wakes other waiting threads
 */
void my_mutex_unlock(int *lock);


/*
 * Handles the alarm event when it occurs (schedule next thread to run)
 */
void handle_alarm(int signal_number);



/*
 *
 */
void my_thread_lock(int *lock);



/*
 * Initializes the lock as unlocked
 */
void my_mutex_init(int *lock);


void my_thread_end();


void my_thread_yield();


void my_thread_join(int thread_id);


/*
 * This function will run when a thread finishes
 * Must remove the element from the list so it cannot be scheduled again
 * Receives the id of the thread that has finished
 */
void end_function(int thread_id);


#endif //MULTIDISPLAY_MY_THREAD_H
