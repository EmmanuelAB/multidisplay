#include <stdio.h>
#include "list.h"

void test_ints(){
    list *a_list = list_create("%d");

    for (int i = 0; i < 10; ++i) {
        list_add_element(a_list, i);
        printf("test %d = %d\n", i, list_get_index_of(a_list, i));
    }

    list_print(a_list);
    list_remove_element_at(a_list, 0);
    list_print(a_list);
    list_remove_element_at(a_list, 1);
    list_print(a_list);
    list_remove_element_at(a_list, 2);
    list_print(a_list);
    list_remove_element_at(a_list, 3);
    list_print(a_list);
    list_remove_element_at(a_list, 5);
    list_print(a_list);

    printf("Deleting invalid index %d\n", list_remove_element_at(a_list, 50));

    printf("Deleting all elements\n");
    while(a_list->size){
        printf("Removing %d \n", list_get_element_at(a_list, 0));
        list_remove_element_at(a_list, 0);
        list_print(a_list);

    }
    printf("All removed\n");
    list_print(a_list);
    printf("Adding one more\n");
    list_add_element(a_list, 10);
    list_print(a_list);
    exit(0);
}

void test_floats(){
    list *a_list = list_create("%.1f");

    for (float i = 0; i < 10; i++) {
        list_add_element(a_list, i);
        printf("test %.1f = %d\n", i, list_get_index_of(a_list, i));
    }

    list_print(a_list);
    list_remove_element_at(a_list, 0);
    list_print(a_list);
    list_remove_element_at(a_list, 1);
    list_print(a_list);
    list_remove_element_at(a_list, 2);
    list_print(a_list);
    list_remove_element_at(a_list, 3);
    list_print(a_list);
    list_remove_element_at(a_list, 5);
    list_print(a_list);

    printf("Deleting invalid index %d\n", list_remove_element_at(a_list, 50));



    list_print(a_list);
    exit(0);
}


int main() {
    test_ints();

//    test_floats();





    return 0;
}