//
// Created by emmanuel on 12/05/19.
//

#ifndef MULTIDISPLAY_CLIENT_H
#define MULTIDISPLAY_CLIENT_H


#include "../sockets/socket_utils.h"
#include "../sockets/port.h"
#include "ncurses.h"
#include "../utils/area.h"

void display_data(char *data, area terminal_size);

void full_terminal(area terminal_size, char symbol);


#endif //MULTIDISPLAY_CLIENT_H
