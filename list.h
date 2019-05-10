//
// Created by emmanuel on 29/04/19.
//

#ifndef MULTIDISPLAY_LIST_H
#define MULTIDISPLAY_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include "tcb.h"

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

list *list_create();

void list_add_element(list *a_list, TYPE element);

void list_print(list *a_list);

int list_get_index_of(list *a_list, TYPE value_looking_for);

int list_remove_element_at(list *a_list, int index);

TYPE list_get_element_at(list *a_list, int index);

#endif //MULTIDISPLAY_LIST_H