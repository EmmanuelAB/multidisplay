#include <stdio.h>
#include <ncurses.h>
#include <pthread.h>

typedef struct point{
    int x;
    int y;
} point;

typedef struct area{
    int width;
    int height;
}area;

void draw_square(point p, area dimensions, char symbol){
    for (int i = 0; i < dimensions.width; ++i) {
        for (int j = 0; j < dimensions.height; ++j) {
            move(p.y + j, p.x + i);
            printw("%c",symbol);
        }
    }
}

void wait_key_press(){getch();}

void draw_first_rect(){
    // the upper left corner is 0,0
    // move takes y offset and x offset in that order
    // any y offset goes downwards and x offset goes rightwards
    int y_offset = 10;
    int x_offset = 40;

    // draw a first rectangle
    move(y_offset, x_offset);
    point p = {x_offset, y_offset};
    area a = {2, 8};
    draw_square(p, a, 'X');
}

void draw_second_rect(){
    // draw a second rectangle
    point p;
    area a;
    p.x = 5;
    p.y = 5;
    a.width = 20;
    a.height = 4;
    draw_square(p,a,'#');

}

int main() {
    initscr();

//    draw_first_rect();
    // the screen content in memory is reflected to the terminal
//    refresh(); wait_key_press(); clear();

//    draw_second_rect()
//    refresh(); wait_key_press(); clear();


    // create windows
    int height=10, width=20, start_y=10, start_x=10;
    WINDOW *window = newwin(height, width, start_y, start_x);
    // make a border to the window
//    box(window, 0, 0);
//    refresh();
    // there's functions to write inside a specific windo
//    wmove(window, 1, 1); // move to the inner section of the window
    wprintw(window, "%s","Hello");
    wrefresh(window);
//    wait_key_press();

    endwin();


    pthread_attr_t foo;


    return 0;
}