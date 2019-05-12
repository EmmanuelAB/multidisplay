//
// Created by emmanuel on 12/05/19.
//
#include "server.h"

int main() {

//    test();

    setbuf(stdout, NULL);

    int socket_fd = start_server();
    printf("Server running\n");
    int clients_sockets[MAX_CLIENTS];
    wait_connections(socket_fd, clients_sockets);
    printf("Server full\n");

    // get desired size
    area dimension;
    ask_terminal_size(&dimension);

    // send the size to all clients
    spread_size(dimension, clients_sockets, MAX_CLIENTS);


    char canvas[dimension.height][dimension.width];
    clean_canvas(canvas, dimension);

    for (int i = 0; i < dimension.width; ++i) {
        canvas[0][i] = 'X';
        printf("Char written\n");
        update_monitors(clients_sockets, canvas, dimension, MAX_CLIENTS); //TODO: pass canvas distributions as param?
        printf("State sent\n");
        sleep(1);
        canvas[0][i] = EMPTY_CHAR;


    }

    char end = 0;
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        send(clients_sockets[i],&end,1,NO_FLAGS);
        close(clients_sockets[i]);
    }

}






