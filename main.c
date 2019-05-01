#include "Language_Parse.h"

/*
Isaac Mena L
27/04/19
After parse the json file you can use the figure_list to iterate over the figures, ask for their
attributes and use them as you want.
-----------------------------
Command to compile and run from shell: gcc main.c Language_Parse.c -ljson-c -o b && ./b
*/

void figure_list_to_string();

int main(){

    printf("\nStarting json parse\n");

    parse_file();

    // Example of printing all the attributes of the figures once they are parsed.
    figure_list_to_string();
    return 0;
}

void figure_list_to_string(){
    for (int i = 0; i < num_objects; i++) {
        printf("\nFigure #%d\n", i+1);
        printf("\nType: %d", figure_list[i].type);
        printf("\nShape:\n%s", figure_list[i].figure);
        printf("\nStart time: %d", figure_list[i].start_time);
        printf("\nEnd time: %d", figure_list[i].end_time);
        printf("\nInitial x: %d", figure_list[i].initial_pos_x);
        printf("\nInitial y: %d", figure_list[i].initial_pos_y);
        printf("\nEnd x: %d", figure_list[i].end_pos_x);
        printf("\nEnd y: %d", figure_list[i].end_pos_y);
        printf("\nInitial angle: %d", figure_list[i].initial_angle);
        printf("\nEnd angle: %d\n", figure_list[i].end_angle);
    }
}
