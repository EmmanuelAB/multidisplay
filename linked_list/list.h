//
// Created by emmanuel on 29/04/19.
//

#ifndef MULTIDISPLAY_LIST_H
#define MULTIDISPLAY_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include "tcb.h"

#define LIST_ELEMENT_NOT_FOUND -1

#define TYPE TCB*

typedef struct node{
    TYPE value;
    struct node *next;
}node;

typedef struct list{
    node *first;
    node *last;
    int size;
    const char *format;
}list;

list *list_create(const char *format);

void list_add_element(list *a_list, TYPE element);

void list_print(list *a_list);

int list_get_index_of(list *a_list, TYPE value_looking_for);

TYPE list_remove_element_at(list *a_list, int index);

TYPE list_get_element_at(list *a_list, int index);

int list_get_index_of_element_with_id(list *a_list, int id);

void sort_max_min(list *a_list);

#endif //MULTIDISPLAY_LIST_H