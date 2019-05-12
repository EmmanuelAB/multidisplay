#include "server.h"

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
    int divided_width = dimension.width / num_of_clients;
    sprintf(message_buffer, "%d,%d",dimension.height, divided_width);
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





void update_monitors(int sockets_to_clients[], char *canvas, area dimension, int num_clients){
    char message[500]; // this will hold all the characters to be sent

    int message_offset = 0;
    int canvas_offset = 0;
    int segment_width = dimension.width / num_clients;

    // send the message to the monitors
    for (int i = 0; i < sizeof(sockets_to_clients); ++i) {
        message_offset = 0;
        // serialize the canvas to message (copy each row to message)

        // copy segments of rows to message
        for (int j = 0; j < dimension.height; ++j) {
            strncpy(message+message_offset, canvas+j*dimension.width+canvas_offset, segment_width);
            message_offset += segment_width;
        }
        send(sockets_to_clients[i], message, strlen(message), NO_FLAGS);
        canvas_offset += segment_width;
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
    char *canvas = "abcdefghijkl";
    area a = {4,3};
    print_char_matrix(canvas,a);
    update_monitors(NULL, canvas, a, MAX_CLIENTS);
    exit(0);
}





