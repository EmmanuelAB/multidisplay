//
// Created by emmanuel on 13/04/19.
//

#include "socket_utils.h"
int create_socket(){

    int socket_fd;

    // Create the socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if ( socket_fd  == ERROR) {
        exit_on_error("Error creating the socket");
    }
    return socket_fd;
}



void bind_socket(int socket_fd, int port){
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons( port );
    address.sin_addr.s_addr = INADDR_ANY;

    int address_length = sizeof(address);

    // Bind the socket to the port
    int result = bind(socket_fd, (struct sockaddr *) &address, address_length );
    if(result == ERROR ){
        close(socket_fd);
        exit_on_error("Error binding the socket to port");
    }
}



void start_listening(int socket_fd, int max_clients){
    int result = listen(socket_fd, max_clients);
    if (result == ERROR){
        close(socket_fd);
        exit_on_error("Error starting to listen");
    }
}



void connect_to_server(int socket_fd, int port){
    //Specify an address for the socket
    struct sockaddr_in client_address;
    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(port);
    client_address.sin_addr.s_addr = INADDR_ANY;

    //Making a connection
    int connection_status = connect(socket_fd, (struct sockaddr *)&client_address, sizeof(client_address));

    if(connection_status == -1){
        close(socket_fd);
        exit_on_error("Error connecting to server");
    }
}
