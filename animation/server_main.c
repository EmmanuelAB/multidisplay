//
// Created by emmanuel on 12/05/19.
//
#include <ncurses.h>
#include "server.h"
#include "../my_thread/my_thread.h"

#define SYMBOL 'X'

int clients_sockets[MAX_CLIENTS];

int socket_fd; // the socket descriptor of the server

area canvas_dimension;

char *canvas;


void paste_canvas(){
    char c;
    for (int i = 0; i < canvas_dimension.height; ++i) {
        for (int j = 0; j < canvas_dimension.width; ++j) {
            move(i,j);
            c = *(canvas + i*canvas_dimension.width + j);
            printw("%c ", c);
        }
    }
    refresh();
}

/*
 * Move a char from left to right in straight-line
 */
void animate_char(int row){
    for (int i = 0; i < 10; ++i) {
        int n = canvas_dimension.width;
        char *cell = canvas + row*n + i;
        *(cell) = SYMBOL;
        paste_canvas();
        usleep(0.1*TO_MICROSECONDS);
        *cell = EMPTY_CHAR;

    }
}



/*
 * Keep sending each section to the corresponding client
 */
void monitor_updater(int param){//not used
    while (1){
        print_char_matrix(canvas, canvas_dimension);
//        update_monitors(clients_sockets, canvas, canvas_dimension, MAX_CLIENTS);
        usleep(0.01*TO_MICROSECONDS);
    }
}



int maina() {

    // start the server
    socket_fd = start_server();
    printf("Server running\n");

    // wait clients
    wait_connections(socket_fd, clients_sockets);
    printf("Server full\n");

    // get desired size
    ask_terminal_size(&canvas_dimension);

    // send the size to all clients
    spread_size(canvas_dimension, clients_sockets, MAX_CLIENTS);


    // clear the canvas to start blank
    canvas  = malloc(canvas_dimension.height * canvas_dimension.width);
    clean_canvas(canvas, canvas_dimension);

    // Launch threads moving 1 char each one

    for (int i = 0; i < 2; ++i) {
        my_thread_create(animate_char, ROUNDROBIN, i);
    }

    // Launch a thread that sends update to the monitors
    my_thread_create(monitor_updater, 0, ROUNDROBIN);


    //TEMP
    while(1);

    // Send "END" message to the monitors
    char end = 0;
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        send(clients_sockets[i],&end,1,NO_FLAGS);
        close(clients_sockets[i]);
    }

}


// testing main
int main(){

    // init library
    my_thread_init();

    // get desired size
    ask_terminal_size(&canvas_dimension);

    initscr();

    // clear the canvas to start blank
    canvas  = malloc(canvas_dimension.height * canvas_dimension.width);
    clean_canvas(canvas, canvas_dimension);


    for (int i = 0; i < 2; ++i) {
        my_thread_create(animate_char, i, ROUNDROBIN);
    }

    // Launch a thread that sends update to the monitors
//    my_thread_create(monitor_updater, 0, ROUNDROBIN);


    //TEMP
    int n=100;
    while(n--){usleep(0.01*TO_MICROSECONDS);}

    endwin();

}






