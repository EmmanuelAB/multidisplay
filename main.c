#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sched.h>

int var = 10;

int thread_function(void *message){
    printf("The child received <%s>\n", (char*)message);
    printf("Child var: %d\n",var);
    printf("Child terminated\n");
    return 0;
}


//int main() {
//    setbuf(stdout, NULL);
//    int STACK_SIZE = 1024*1024;
//    void *stack = malloc(STACK_SIZE);
//    char *arg = "HELLO";
//
//    var = 1000;
//
//    int flags = SIGCHLD | CLONE_VM;
//    int thread_id = clone(thread_function, stack+STACK_SIZE, flags ,arg);
//    printf("Created thread id:%d\n", thread_id);
//    printf("Parent pid:%d\n", getpid());
//    printf("Parent var: %d\n",var);
//    wait(NULL); //wait child to terminate
//    free(stack);
//    return 0;
//}


void signal_received(int signal_number){
    if (signal_number == SIGTSTP) { // this signal can be caught
        printf("Child stopped\n");
        kill(getpid(), SIGSTOP); // this can't and this do stop the process
    }
    else if(signal_number == SIGCONT)
        printf("Child continued\n");
    else
        printf("Unknown signal received %d\n", signal_number);
}


//child func counts from 1 to inf with delay between prints
int start_counting(){
    // set the signal handlers
    signal(SIGCONT, signal_received);
    signal(SIGTSTP, signal_received);
    for (int i = 0; i < 1000; ++i) {
        printf("%d\n",i);
        sleep(1);
    }
    return 0; // exit status
}

int launch_child(void *stack, int stack_size){

    int flags = SIGCHLD;
    int thread_id = clone(start_counting, stack+stack_size, flags ,NULL);
    printf("Created thread id:%d\n", thread_id);
    return thread_id;
}


int main(){
    setbuf(stdout, NULL);
    int stack_size = 1024*1024;
    void *stack = malloc(stack_size);

    // store the state of the child (1=running, 0=stopped)
    int child_state = 1;

    // launch counter child
    int child_id = launch_child(stack, stack_size);

    char input_string[2];
    while(1){

        // ask user input
        printf("Enter x to finish or t to toggle state\n");
        scanf("%s",input_string);

        // if first letter is x then exit loop
        if( input_string[0] == 'x'){
            kill(child_id, SIGTSTP);
            break;
        }

        // else toggle the state of the child
        if (child_state == 1){
            // send stop signal
            kill(child_id, SIGTSTP);
        }else{
            //send continue signal
            kill(child_id, SIGCONT);
        }
        child_state = 1 - child_state;
    }


    // free child's stack
    free(stack);
}