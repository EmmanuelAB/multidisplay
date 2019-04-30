struct process{
    char *name;
    int execution_time;
    int tickets_number;
    int period;
    struct process *next;
};

struct process_list{
    struct process *first;
    struct process *last;
    int elements;
};


struct process *create_element(char *name,int tickets, int period){
    struct process *new_node = malloc(sizeof(struct process));
    new_node->name = name;
    new_node->execution_time = tickets;
    new_node->tickets_number = tickets;
    new_node->period = period;
    new_node->next = NULL;

    return new_node;
}

struct process_list *add_element(struct process_list *list, char *name, int tickets,int period){

    struct process *new_node = create_element(name,tickets,period);
    list->elements++;

    if (list->first != NULL){
        list->last->next = new_node;
        list->last = new_node;


    }else{
        list->first = new_node;
        list->last = new_node;
    }

}


struct process_list *delete_element(struct process_list *list, int id){
    struct process* temp = list->first;

    if (temp == NULL){
        printf("lista vacia\n");
    }

    if (id == 0){
        list->first = temp->next;
        free(temp);
        return 0;
    }

    // Find previous node of the node to be deleted
    for (int i=0; temp!= NULL && i<id-1; i++)
        temp = temp->next;

    // If la posicion es mas grande que el numero de nodos
    if (temp == NULL || temp->next == NULL)
        return 0;

    struct process *next = temp->next->next;

    free(temp->next);  // Free memory

    temp->next = next;  // Unlink the deleted node from list
}

struct process *search_element(struct process_list *list, int id){
    struct process *aux;
    aux = list->first;
    int counter = 0;

    while (counter != id) {
        aux = aux->next;
        counter++;
    }
    return aux;
}

struct process_list *sort_max_min(struct process_list *list){
    if (list != NULL){
        struct process *pivote = NULL, *actual= NULL;
        pivote = list->first;
        int aux = 0;

        while(pivote != NULL){
            actual = pivote->next;

            while(actual != NULL){
                if (pivote->tickets_number < actual->tickets_number){
                    aux = pivote->tickets_number;

                    pivote->tickets_number = actual->tickets_number;
                    actual->tickets_number = aux;
                }
                actual = actual->next;
            }
            pivote = pivote->next;
        }


    }else{
        printf("Lista vacia o no inicializada\n");
    }
}


void show_list(struct process_list *list) {
    struct process *aux;
    aux = list->first;

    if (aux == NULL) printf( "\nLa lista está vacía!!\n" );

    while (aux != NULL) {
        printf( "CPU: %d\n", aux->execution_time);
        //printf( "CPU: %d\n", aux->tickets_number);
        aux = aux->next;
    }
}
