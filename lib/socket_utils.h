//
// Created by emmanuel on 13/04/19.
//

#ifndef MULTIDISPLAY_SOCKET_UTILS_H
#define MULTIDISPLAY_SOCKET_UTILS_H

#include <arpa/inet.h>
#include "error_handling.h"
#include <unistd.h>


/*
	Creates a socket and return the socket file descriptor
	*If there is any error then exits the program with corresponding message
*/
int create_socket();



/*
	If there is any error then exits the program with
	corresponding message
*/
void bind_socket(int socket_fd, int port);




/*
   *If there is any error then exits the program with corresponding message
*/
void start_listening(int socket_fd, int max_clients);



/*
    It connects to localhost at received port
    *If there is any error then exits the program with corresponding message
*/
void connect_to_server(int socked_fd, int port);

#endif //MULTIDISPLAY_SOCKET_UTILS_H
