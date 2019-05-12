//
// Created by emmanuel on 13/04/19.
//

#include "error_handling.h"


void exit_on_error(const char *error){
    printf("[ERROR] %s \n", error);
    print_error_status();
    printf(">>> Program finished.\n");
    exit(1);
}



void print_error_status(){
    printf("Error status: %s\n", strerror(errno));
}
