#ifndef MULTIDISPLAY_LANGUAGE_PARSE_H
#define MULTIDISPLAY_LANGUAGE_PARSE_H

#include <stdio.h>
#include <json-c/json.h>
#include <string.h>

#define BUFFER_SIZE 2048
#define MAX_FIGURES_ALLOWED 25

struct figure {
    int type;
    char* figure;
    int start_time;
    int end_time;
    int initial_pos_x;
    int initial_pos_y;
    int end_pos_x;
    int end_pos_y;
    int initial_angle;
    int end_angle;
};

int num_objects;
struct figure figure_list[MAX_FIGURES_ALLOWED];
struct json_object *parsed_json;
struct json_object *json_figure_list;
char buffer[BUFFER_SIZE];

void parse_file();
void read_file();
void set_figure_list();
void set_figure_from_list();
char* parse_figure_file(const char* filename);
struct figure build_figure_from_json_figure(struct json_object *figure);

#endif //MULTIDISPLAY_LANGUAGE_PARSE_H
