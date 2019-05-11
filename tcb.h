//
// Created by emmanuel on 09/05/19.
//

#ifndef MULTIDISPLAY_TCB_H
#define MULTIDISPLAY_TCB_H

#include <ucontext.h>

typedef struct TCB {
    int id;
    int scheduler;
    int state;
    int waiting_thread_id;
    int blocked;
    int ready;
    ucontext_t *context;
} TCB;

#endif //MULTIDISPLAY_TCB_H
