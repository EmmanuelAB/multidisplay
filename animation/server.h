//
// Created by emmanuel on 12/05/19.
//

#ifndef MULTIDISPLAY_SERVER_H
#define MULTIDISPLAY_SERVER_H

#include <stdio.h>
#include "../sockets/socket_utils.h"
#include "../sockets/port.h"
#include "../utils/area.h"
#include "../utils/point.h"

#define MAX_CLIENTS 2
#define EMPTY_CHAR '-'

int start_server();


/*
 * Read a port from a fixed file
 */
int read_port();



/*
 * Returns until -MAX_CLIENTS- are received, the sockets of the clients are written to the array received
 */
int wait_connections(int socket_fd, int *sockets);


/*
 * Asks both desired dimensions to the user and stores the input at the struct
 */
void ask_terminal_size(area *dimension);


/*
 * Send the assigned terminal size to each client in the array
 */
void spread_size(area dimension, int *clients_sockets, int num_of_clients);


/*
 * Fulfills the canvas with EMPTY_CHAR's
 */
void clean_canvas(char *canvas, area dimension);


/*
 * Prints the given matrix in tabular form
 * It assumes all the character are contiguous in straight-line
 */
void print_char_matrix(char *matrix, area dimension);



/*
 * Splits the canvas into equally sized parts and send each part to a client in the array
 */
void update_monitors(int sockets_to_clients[], char *canvas, area dimension, int num_clients);



#endif //MULTIDISPLAY_SERVER_H
