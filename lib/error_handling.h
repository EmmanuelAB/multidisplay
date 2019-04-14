//
// Created by emmanuel on 13/04/19.
//

#ifndef MULTIDISPLAY_ERROR_HANDLING_H
#define MULTIDISPLAY_ERROR_HANDLING_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define ERROR -1



/*
	Prints error followed by the name of errno and finally
	exits the program with code 1
*/
void exit_on_error(const char* error);



/*
    It pints the current error held by errno
*/
void print_error_status();



#endif //MULTIDISPLAY_ERROR_HANDLING_H
