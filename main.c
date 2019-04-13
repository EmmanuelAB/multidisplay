#include <stdio.h>
#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>

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

void hide_terminal_cursor(){
    curs_set(0);
}

void my_sleep(float seconds){
    usleep(seconds*1000000);
}

int main() {
    initscr();

    hide_terminal_cursor();
    char symbol = 'X';
    point start = {0,0}, end = {98,26}, current_position = {start.x, start.y};
    area a = {4,4};

    while( current_position.x <= end.x && current_position.y <= end.y ){
        // write the char
        draw_square(current_position,a,symbol);
        refresh();
        my_sleep(0.1);
        // clear the char
        draw_square(current_position,a,' ');
        refresh();
        current_position.y++;
        current_position.x += 4;
    }


    endwin();
    return 0;
}