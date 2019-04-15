#include <stdio.h>
#include <json-c/json.h>

int main() {
    char buffer[1024];

    FILE *file = fopen("../example.json","r");
    fread(buffer, sizeof(buffer), 1, file);
    fclose(file);

    struct json_object *parsed_json = json_tokener_parse(buffer);

    struct json_object *angle;

    json_object_object_get_ex(parsed_json,"final_angle", &angle);

//    printf("Angle is %s \n", json_object_get_string(angle));
    printf("Angle is %i \n", json_object_get_int(angle));


    return 0;
}