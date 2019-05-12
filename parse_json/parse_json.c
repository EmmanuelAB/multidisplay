#include "parse_json.h"

char* file_name = "../TestingFiles/example.json";

void read_file(){
    printf("\nReading file\n");
    FILE *json_file = fopen(file_name, "r");
    fread(buffer, BUFFER_SIZE, 1, json_file);
    fclose(json_file);
}

void set_figure_list(){
    printf("\nSetting figure list\n");
    parsed_json = json_tokener_parse(buffer);
    json_object_object_get_ex(parsed_json, "objects", &json_figure_list);
    num_objects = json_object_array_length(json_figure_list);
    printf("\nThe number of objects is %d\n", num_objects);
}

void set_figure_from_list(){
    printf("\nSetting each figure in list\n");
    struct json_object *current_json_figure;
    struct figure current_figure;
    for (int i = 0; i < num_objects; i++) {
        current_json_figure = json_object_array_get_idx(json_figure_list, i);
        current_figure = build_figure_from_json_figure(current_json_figure);
        figure_list[i] = current_figure;
    }
}

struct figure build_figure_from_json_figure(struct json_object *json_figure){

    printf("\nBuilding figure\n");

    struct json_object *type;
    struct json_object *figure_file_name;
    struct json_object *start_time;
    struct json_object *end_time;
    struct json_object *initial_pos_x;
    struct json_object *initial_pos_y;
    struct json_object *end_pos_x;
    struct json_object *end_pos_y;
    struct json_object *initial_angle;
    struct json_object *end_angle;
    struct json_object *rows;
    struct json_object *cols;

    json_object_object_get_ex(json_figure, "type", &type);
    json_object_object_get_ex(json_figure, "figure_file", &figure_file_name);
    json_object_object_get_ex(json_figure, "start_time", &start_time);
    json_object_object_get_ex(json_figure, "end_time", &end_time);
    json_object_object_get_ex(json_figure, "initial_pos_x", &initial_pos_x);
    json_object_object_get_ex(json_figure, "initial_pos_y", &initial_pos_y);
    json_object_object_get_ex(json_figure, "end_pos_x", &end_pos_x);
    json_object_object_get_ex(json_figure, "end_pos_y", &end_pos_y);
    json_object_object_get_ex(json_figure, "initial_angle", &initial_angle);
    json_object_object_get_ex(json_figure, "end_angle", &end_angle);
    json_object_object_get_ex(json_figure, "rows", &rows);
    json_object_object_get_ex(json_figure, "cols", &cols);

    struct figure current_figure;
    current_figure.type = json_object_get_int(type);

    // Read the figure file and save it in the structure
    const char* figure_file = json_object_get_string(figure_file_name);
    current_figure.figure = parse_figure_file(figure_file);

    current_figure.start_time= json_object_get_int(start_time);
    current_figure.end_time = json_object_get_int(end_time);
    current_figure.initial_pos_x= json_object_get_int(initial_pos_x);
    current_figure.initial_pos_y = json_object_get_int(initial_pos_y);
    current_figure.end_pos_x= json_object_get_int(end_pos_x);
    current_figure.end_pos_y = json_object_get_int(end_pos_y);
    current_figure.initial_angle= json_object_get_int(initial_angle);
    current_figure.end_angle = json_object_get_int(end_angle);
    current_figure.rows = json_object_get_int(rows);
    current_figure.cols = json_object_get_int(cols);

    return current_figure;

}

char* parse_figure_file(const char* filename){
    printf("\nReading figure file\n");
    char path[30];
    sprintf(path, "%s%s", "../TestingFiles/", filename);
    FILE* file = fopen(path, "r");
    static char content[BUFFER_SIZE];
    fread(content, BUFFER_SIZE, 1, file);
    fclose(file);
    return content;
}

void parse_file(){
    read_file();
    set_figure_list();
    set_figure_from_list();
}
