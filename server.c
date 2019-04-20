#include <stdio.h>
#include "lib/socket_utils.h"
#include "port.h"
#include "lib/area.h"
#include "lib/point.h"

#define MAX_CLIENTS 2

int start_server(){
    int socket_fd = create_socket();
    bind_socket(socket_fd, PORT);
    start_listening(socket_fd, PORT);
    return socket_fd;

}

int wait_connections(int socket_fd, int *sockets){
    int arrived_clients = 0;
    int index_to_write = 0;
    while(arrived_clients < MAX_CLIENTS){
        int new_client_socket_fd = accept(socket_fd, NULL, NULL);
        *(sockets + index_to_write) = new_client_socket_fd;
        arrived_clients++;
        index_to_write++;
        printf("New client\n");
    }  
}

void ask_terminal_size(area *dimension){
    printf("Enter terminal heigth:");
    scanf("%d", &dimension->height);
    printf("Enter terminal width:");
    scanf("%d", &dimension->width);
}

void spread_size(area dimension, int *clients_sockets, int num_of_clients){
    char message_buffer[100];
    sprintf(message_buffer, "%d,%d",dimension.height, dimension.width);
    for (int i = 0; i < num_of_clients; ++i) {
        send(clients_sockets[i], message_buffer, sizeof(message_buffer), NO_FLAGS);
    }
}

int main() {

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
}