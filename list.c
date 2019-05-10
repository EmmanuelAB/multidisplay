//
// Created by emmanuel on 29/04/19.
//

#include "list.h"

list *list_create(const char *format){
    list *a_new_list = malloc(sizeof(list));
    a_new_list->first = NULL;
    a_new_list->last = NULL;
    a_new_list->size = 0;
    a_new_list->format = format;
    return a_new_list;
}

void list_add_element(list *a_list, TYPE element){
    // create a new node to insert
    node *new_node = malloc(sizeof(node));

    // set the end of the list
    new_node->next = NULL;

    // set the value of the new node
    new_node->value = element;

    if(a_list->first == NULL)
        a_list->last = a_list->first = new_node;

    else
        a_list->last->next = new_node;// link the new element
    a_list->last = new_node;

    // update list size
    a_list->size++;
}


void list_print(list *a_list){
    node *current = a_list->first;
    printf("[");
    while(current != NULL){
        // print current element
        printf(a_list->format, current->value);

        if (current->next != NULL){
            // there is elements left
            printf(", ");
        }

        // pass to the next element
        current = current->next;
    }
    printf("]\n");
}

// return 0 if found and deleted or -1 if no found
int list_remove_element_at(list *a_list, int index){
    int return_value = -1;
    int current_index = 0;
    int element_deleted = 0;
    node *current = a_list->first;
    node *prior_node = current;
    while (current != NULL && ! element_deleted) {
        if(current_index == index){
            // special_case
            if(index == 0)
                a_list->first = current->next;
            else
                prior_node->next = current->next;
            a_list->size--;
            element_deleted = 1;
            return_value = 0;
            free(current);

        }else{
            prior_node = current;
            current = current->next;
            current_index++;
        }
    }
    return return_value;
}

int list_get_index_of(list *a_list, TYPE value_looking_for) {
    int return_index = -1;
    int current_index = 0;
    node *current = a_list->first;
    while (current != NULL && return_index == -1) {
        if(current->value == value_looking_for)
            return_index = current_index;
        current = current->next;
        current_index++;
    }
    return return_index;
}

TYPE list_get_element_at(list *a_list, int index){
    TYPE return_value;
    int current_index = 0;
    int element_found = 0;
    node *current = a_list->first;
    while (current != NULL && ! element_found) {
        if(current_index == index)
            return_value = current->value;
        current = current->next;
        current_index++;
    }
    return return_value;
}