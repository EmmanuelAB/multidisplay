//
// Created by emmanuel on 12/05/19.
//
#include <ncurses.h>
#include "server.h"
#include "../my_thread/my_thread.h"
#include <math.h>
#include "../parse_json/parse_json.h"

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
    for (int i = 0; i < canvas_dimension.width; ++i) {
        int n = canvas_dimension.width;
        char *cell = canvas + row*n + i;
        *(cell) = SYMBOL;
//        paste_canvas();
        usleep(0.05*TO_MICROSECONDS);
        *cell = EMPTY_CHAR;

    }
    my_thread_end();
}



/*
 * Keep sending each section to the corresponding client
 */
void monitor_updater(int param){//not used
    while (1){
//        print_char_matrix(canvas, canvas_dimension);
        update_monitors(clients_sockets, canvas, canvas_dimension, MAX_CLIENTS);
        usleep(0.05*TO_MICROSECONDS);
    }
}

float eval(float m, float x, float b){ return m*x+b;}


void calc_func(fpoint a, fpoint b, float *m, float *be){
    *m = (b.y - a.y) / (b.x - a.x);
    *be = a.y - (*m) * a.x;
}


void animate_simple(int index){
    int y, n = canvas_dimension.width;
    for (int x = 0 ; x < n ; ++x) {
        y = (int) round((double) eval(m, x, be));
        move( -1*y, x); //remove the negative

        char *cell = canvas + y*n + x;
        *(cell) = SYMBOL;
        usleep(0.05*TO_MICROSECONDS);
        *cell = EMPTY_CHAR;
    }

    my_thread_end();
}

void animate_object( int index ){
    fpoint ini, end;
    ini.x = (float)figure_list[index].initial_pos_x;
    ini.y = (float)figure_list[index].initial_pos_y;

    end.x = (float)figure_list[index].end_pos_x;
    end.y = (float)figure_list[index].end_pos_y;

    printf("animating from %.1f,%.1f to %.1f,%.1f\n", ini.x, ini.y, end.x, end.y);

    end.y *= -1; // invert to negative
    ini.y *= -1; // to calculate the linear function criteria


    float m, be;
    calc_func(ini,end,&m,&be);

    int y, n = canvas_dimension.width;
    for (int x = ini.x; x < end.x; ++x) {
        y = (int) round((double) eval(m, x, be));
        move( -1*y, x); //remove the negative

        char *cell = canvas + y*n + x;
        *(cell) = SYMBOL;
        usleep(0.05*TO_MICROSECONDS);
        *cell = EMPTY_CHAR;
    }

    my_thread_end();
}

int main() {

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

    // init the library
    my_thread_init();


    // parse json file to get the paths to animate objects
    parse_file();

    // store ids of the animators
    list *ids = list_create("%d");
    int animator_id;

    // launch threads moving 1 char each one
    for (int i = 0; i < num_objects; ++i) {
//    for (int i = 0; i < 1; ++i) {
        animator_id = my_thread_create(animate_object, i, ROUNDROBIN);
        printf("animator started, id:%d\n", animator_id);
        list_add_element(ids, animator_id);
    }

    // Launch a thread that sends update to the monitors
    my_thread_create(monitor_updater, 0, ROUNDROBIN);


    my_thread_join(1);

//    for (int i = 0; i < ids->size; ++i) {
//        my_thread_join((int)list_get_element_at(ids, i));

//    }

    printf("main finished joins\n");


    // Send "END" message to the monitors
    char end = 0;
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        send(clients_sockets[i],&end,1,NO_FLAGS);
        close(clients_sockets[i]);
    }

}








