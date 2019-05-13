//
// Created by emmanuel on 12/05/19.
//
#include <ncurses.h>
#include "server.h"
#include "../my_thread/my_thread.h"
#include "../parse_json/parse_json.h"
#include <math.h>

#define SYMBOL 'X'

int clients_sockets[MAX_CLIENTS];

int socket_fd; // the socket descriptor of the server

area canvas_dimension;

int *locks; // a matrix of mutexes that help to synchronize the object and avoid crashes between them

char *canvas;

typedef struct fpoint{
    float x;
    float y;
} fpoint;



/*
 * Prints the matrix to the ncurses screen
 */
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



void calc_func(fpoint a, fpoint b, float *m, float *be){
    *m = (b.y - a.y) / (b.x - a.x);
    *be = a.y - (*m) * a.x;
}



int *get_lock_at(int row, int column){
    return locks + (row*canvas_dimension.width + column)*sizeof(int);

}




float eval(float m, float x, float b){ return m*x+b;}




void draw_line(fpoint ini, fpoint end){

    end.y *= -1; // invert to negative
    ini.y *= -1; // to calculate the linear function criteria

    float m, be;
    calc_func(ini,end,&m,&be);

    int y;
    for (int x = ini.x; x < end.x; ++x) {
        y = (int) round((double)eval(m, x, be));
        move( -1*y, x); //remove the negative
        printw("%c",'X');
    }

    refresh();

}




void left_to_right(){
    int ini_value = 0, end_value = canvas_dimension.width;
    int row = 0;
    int *lock;
    for (int i = ini_value; i < end_value; ++i) {
        int n = canvas_dimension.width;

        // lock the cell before writing to it
        lock = get_lock_at(row, i);
        my_mutex_lock(lock);

        // write to the cell
        char *cell = canvas + row*n + i;
        *(cell) = SYMBOL;

        usleep(0.05*TO_MICROSECONDS);

        *cell = EMPTY_CHAR;

        // unlock the cell
        my_mutex_unlock(lock);

    }
}




void right_to_left(){
    int ini_value = canvas_dimension.width, end_value = 0;
    int row = 0;
    int *lock;
    for (int i = ini_value; i >= end_value; --i) {
        int n = canvas_dimension.width;

        // lock the cell before writing to it
        lock = get_lock_at(row, i);
        my_mutex_lock(lock);

        // write to the cell
        char *cell = canvas + row*n + i;
        *(cell) = SYMBOL;

        usleep(0.05*TO_MICROSECONDS);

        *cell = EMPTY_CHAR;

        // unlock the cell
        my_mutex_unlock(lock);

    }
}







/*
 * Sets -locks- matrix and initializes each value.
 * -canvas_dimension- must be already initialized
 */
void init_locks(){
    locks = malloc(canvas_dimension.width*canvas_dimension.height*sizeof(int));
    int *current_lock;
    for (int i = 0; i < canvas_dimension.height; ++i) {
        for (int j = 0; j < canvas_dimension.width; ++j) {
            current_lock = get_lock_at(i, j);
            my_mutex_init(current_lock);
//            printf("curr lock: %d\n",*current_lock);
        }
    }
}



// testing main
int main(){
    canvas_dimension.width = 210;
    canvas_dimension.height = 50;

    initscr();

    init_locks();


    my_thread_init();

    canvas = malloc(canvas_dimension.height * canvas_dimension.width);
    clean_canvas(canvas, canvas_dimension);
    paste_canvas();

//     Launch a thread that moves from left to right
    my_thread_create(left_to_right, 1, ROUNDROBIN);

    // other direction
//    my_thread_create(right_to_left, 1, ROUNDROBIN);

    //close a lock in the path
    my_mutex_lock(get_lock_at(0, 50));

//    while(1){}

    for(int n=0;n<300;n++){
//        refresh();
        paste_canvas();
        usleep(0.1*TO_MICROSECONDS);
    }

    getchar();
    endwin();

    return 0;

}





