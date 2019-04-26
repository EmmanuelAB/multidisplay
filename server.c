#include <stdio.h>
#include "lib/socket_utils.h"
#include "port.h"
#include "lib/area.h"
#include "lib/point.h"

#define MAX_CLIENTS 2
#define EMPTY_CHAR '-'


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

void clean_canvas(char *canvas, area dimension) {
    for (int i = 0; i < dimension.height; ++i) {
        for (int j = 0; j < dimension.width; ++j) {
            *(canvas+i*dimension.width+j ) = EMPTY_CHAR;
        }
    }
}

void print_char_matrix(char *matrix, area dimension){
    char c;
    for (int i = 0; i < dimension.height; ++i) {
        for (int j = 0; j < dimension.width; ++j) {
            c = *(matrix+i*dimension.width + j);
            printf("%c ",c);
        }
        printf("\n");
    }
}

void update_monitors(int sockets_to_clients[], char *canvas, area dimension){
    char message[500]; // this will hold all the characters to be sent

    // serialize the canvas to message (copy each row to message)
    int offset = 0;
    for (int i = 0; i < dimension.height; i++) {
        strncpy(message+offset, canvas+offset, dimension.width);
        offset += dimension.width;
    }


    // send the message to the monitors
    for (int i = 0; i < sizeof(sockets_to_clients); ++i) {
        send(sockets_to_clients[i], message, strlen(message), NO_FLAGS);
    }
}


void test(){
    // test cleaning canvas
//    setbuf(stdout, NULL);
//    area dimension = {4,4};
//    char canvas[dimension.height][dimension.width];
//    print_char_matrix(canvas, dimension);
//    clean_canvas(canvas, dimension);
//    print_char_matrix(canvas, dimension);
//    exit(0);

    // test serializing matrix
    char *canvas = "abcdefghi";
    area a = {3,3};
    print_char_matrix(canvas,a);

    update_monitors(NULL, canvas, a);
    exit(0);
}

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
        update_monitors(clients_sockets, canvas, dimension); //TODO: pass canvas distributions as param?
        printf("State sent\n");
        sleep(1);
        canvas[0][i] = EMPTY_CHAR;


    }

    char end = 0;
    send(clients_sockets[0],&end,1,NO_FLAGS);

}
