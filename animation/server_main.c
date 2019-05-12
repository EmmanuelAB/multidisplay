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

char *canvas;

typedef struct fpoint{
    float x;
    float y;
} fpoint;


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


}


void calc_func(fpoint a, fpoint b, float *m, float *be){
    *m = (b.y - a.y) / (b.x - a.x);
    *be = a.y - (*m) * a.x;
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

// testing main
int main(){
    initscr();

    getmaxyx(stdscr, canvas_dimension.height, canvas_dimension.width);

    canvas  = malloc(canvas_dimension.height * canvas_dimension.width);
    clean_canvas(canvas, canvas_dimension);
    paste_canvas();


    //parse json
    parse_file();

    //iterate all objects
    fpoint ini, end;
    for (int i = 0; i < num_objects; ++i) {
        ini.x = (float)figure_list[i].initial_pos_x;
        ini.y = (float)figure_list[i].initial_pos_y;

        end.x = (float)figure_list[i].end_pos_x;
        end.y = (float)figure_list[i].end_pos_y;

        draw_line(ini, end);

    }



    getchar();
    endwin();

}






