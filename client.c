//
// Created by emmanuel on 20/04/19.
//

#include "lib/socket_utils.h"
#include "port.h"
#include "ncurses.h"
#include "lib/area.h"


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


int main(){

    // create socket
    int socket_fd = create_socket();

    // connect to server
    connect_to_server(socket_fd, PORT);

    initscr();

    // get the current terminal size
    area current_size;
    getmaxyx(stdscr, current_size.height, current_size.width);


    // full the screen with some char
    full_terminal(current_size, 'X');
    refresh();

    // read assigned dimension
    char buffer[500];
    recv(socket_fd, buffer, sizeof(buffer), NO_FLAGS);
    area assigned_size;
    sscanf(buffer,"%d,%d", &assigned_size.height, &assigned_size.width);


    // resize terminal
    resizeterm(assigned_size.height, assigned_size.width);
    refresh();

    // start receiving data from the server
    int bytes_received;
    while(1){
        bytes_received = recv(socket_fd, buffer, sizeof(buffer), NO_FLAGS);
        if(bytes_received == 1) break;
        display_data(buffer, assigned_size);
    }
    // close connection
    close(socket_fd);

    endwin();
    
    printf("End of animation\n");


    return 0;
}
