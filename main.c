#include <unistd.h>
#include <ncurses.h>
#include "my_thread.h"


void function(int row){
    int n = 1;
    while(1){
        move(row,0);
        printw("%d", n++);
        refresh();
        usleep(0.25 * TO_MICROSECONDS);
    }
}


int main() {

    initscr();

    my_thread_init();

    my_thread_create( function, 1);
    my_thread_create( function, 2);
    my_thread_create( function, 3);


    while(1){
//        printf("Main doing nothing\n");
        usleep(0.25*TO_MICROSECONDS);
    }

    endwin();
    
    return 0;

}