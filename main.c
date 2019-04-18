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

void handle_signal(int singal_number){
    printf("Alarm!\n");
    alarm(1);
}


void setup_signal() {
    struct sigaction action;
    action.sa_handler = handle_signal;
    sigemptyset (&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGALRM, &action, NULL); // NULL is the old action, not used here
    alarm(1);
    while(1) {
        printf("Sleeping...\n");
        usleep(0.3 * 1000000);

    }
}


int main() {

    setup_signal();


    return 0;

}