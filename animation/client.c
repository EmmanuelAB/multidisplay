//
// Created by emmanuel on 12/05/19.
//

#include "client.h"

//
// Created by emmanuel on 20/04/19.
//

void display_data(char *data, area terminal_size) {
    int i = 0; // index to
    for (int row = 0; row < terminal_size.height; row++) {
        for (int column = 0; column < terminal_size.width ; column++) {
            move(row, column);
            printw("%c", data[i]);
            i++;
        }
    }
    refresh();
}

void full_terminal(area terminal_size, char symbol){
    for (int row = 0; row < terminal_size.height; ++row) {
        for (int column = 0; column < terminal_size.width; ++column) {
            move(row, column);
            printw("%c", symbol);
        }
    }
}
