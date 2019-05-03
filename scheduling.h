#include<stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include "test_animation.h"
#include "simple_list.h"

//Functions

void round_robin(struct process_list *list, int quantum);
void scheduling_lottery(struct process_list *list);

void real_time(struct process_list *list);
bool utilization_test(struct process_list *list);
int least_common_multiple(struct process_list *list);
int prior_process(struct process_list *list,int lcm, int number_process);
int get_random(int from, int to);
int process_winner(struct process_list *list);


#define NUM_PROCESS 5 //Cantidad de procesos a ejecutar
#define QUANTUM 5 //Tiempo o chance que se le dara a cada proceso para ajecutarse
int total_tickets = 40;

/*
*/
void round_robin(struct process_list *list, int quantum){
    printf("\n................................\n");
    printf("...........ROUND ROBIN..........\n");
    printf("................................\n\n");

    //response time -> tiempo de respuesta de cada proceso
    double response_time = 0, average_response_time = 0;// tiempo promedio.
    int finish = 0, absolute_value, number_quantum = 0, i=0, number_process;

    struct process *current;

    // Algoritmo Round Robin
    while(finish ==0){
        finish=1;
        number_process = list->elements - 1;
        i=0;
        current = list->first;

        while(number_process >= 0) {
            if(current->execution_time > 0) {

                //Para casos especiales, en caso de que el quantum sea mayor o igual al total de cpu ocupado por
                //el proceso
                if (current->execution_time <= quantum){
                    //current->execution_time -= quantum;
                    //absolute_value = fabs(current->execution_time);

                    //response_time += quantum - absolute_value;
                    current->execution_time = 0;

                }else{
                    current->execution_time -= quantum;
                    //response_time += quantum;

                }
                number_quantum++; // numero de quantum


                printf("\nQuantum[%d] process %s",number_quantum, current->name);

                if (current->execution_time > 0) {
                    finish = 0; //No finish
                    i++;

                }else{
                    //average_response_time += response_time;
                    printf("\nThe process %s finish!\n",current->name);
                    delete_element(list,i);
                    list->elements--;
                }
                current = current->next;
                number_process--;
            }
        }
    }
}

/*
*/
void scheduling_lottery(struct process_list *list){
    printf("\n................................\n");
    printf(".......SCHEDULING LOTTERY.......\n");
    printf("................................\n\n");

    int winner;
    int finish = list->elements -1;
    struct process *current;

    sort_max_min(list);

    while(finish >=0){
        winner = process_winner(list);
        current = search_element(list,winner);

        printf("winner - %s\n",current->name);

        total_tickets -= current->tickets_number;
        delete_element(list,winner);

        finish--;
    }

    printf("\nThere aren't more processes\n");
}

/*
*/
int get_random(int from, int to){
    int numero = rand () % (to + 1) + from;
    return numero;
}

/*
Retorna el id del proceso que gano la cpu
*/
int process_winner(struct process_list *list){
    int winner = get_random(0,total_tickets);
    int suma = 0, i = 0;

    struct process *current = list->first;

    while(current->next != NULL){
        suma += current->tickets_number;
        current = current->next;

        if(suma > winner){
            break;
        }
        i++;
    }
    return i; //id del proceso ganador
}


/*
*/
//RMS
//https://en.wikipedia.org/wiki/Rate-monotonic_scheduling
void real_time(struct process_list *list){
    printf("\n................................\n");
    printf("....RATE MONOTONIC SCHEDULING...\n");
    printf("................................\n\n");

    int number_process = list->elements;
    struct process *current = list->first;

    //lcm -> least common multiple
    int lcm, time, i, prior , initial_array[2][number_process];

    //Is the system sheculable?
    if (utilization_test(list)){
        printf("\nThe system is schedulable!\n\n\n");

    }else{
        printf("\nNo, isn't sure!\n\n");
    }

    lcm = least_common_multiple(list);

    //Necesitamos arreglo para guardar valores iniciales
    //esto para reiniciar cada proceso cuando su periodo llegue a su fin
    for(i=0; i < number_process; i++){
        initial_array[0][i] = current->execution_time;
        initial_array[1][i] = current->period;
        current = current->next;
    }


    //Algoritmo rate monotonic scheduler
    for(time = 0; time < lcm; time++){
        current = list->first;
        //Condicion para saber si es seguro ejecutar estos procesos con Rate monotonic scheduler
        //El tiempo de ejecucion nunca debe ser mayor al periodo
        for(i= 0; i < number_process; i++){
            if(current->execution_time > current->period){
                printf("\nThe system is not schedulable because the task %d can't finish its job in its specified period",i);
                return;
            }
            current = current->next;
        }

        //obtengo proceso con mayor prioridad para ejecutarse
        prior = prior_process(list,lcm, number_process);

        if(prior != -1){//execute task
            current = search_element(list, prior);
            printf("(%d-%d)| %s\n", time,time+1, current->name);
            current->execution_time--;
            //mover_derecha(process[prior].name);
            //sleep(2);
        }
        else{//no execute task
            printf("(%d-%d)\n", time, time+1);
        }

        //reinicar
        current = list->first;
        //Cada segundo que pasa se lo voy restando al periodo actual de todos los procesos 1s
        for(i= 0; i< number_process; i++){
            current->period--;

            //If el periodo de un proceso ya termino lo reinicio con sus valores iniciales
            if(current->period == 0){
                current->execution_time = initial_array[0][i];
                current->period = initial_array[1][i];
            }
            current = current->next;
        }
    }
}

/*
*/
int least_common_multiple(struct process_list *list){
    int number_process = list->elements;
    int initial_array[number_process], return_array[number_process];

    struct process *current = list->first;

    for(int i=0; i < number_process; i++){
        initial_array[i] = current->period;
        return_array[i] = current->period;
        current = current->next;
    }

    int index, max, first, flag = 1, i;

    while(flag == 1){
        flag = 0;
        first = return_array[0];
        max = return_array[0];
        index = 0;

        for(i = 1; i < number_process; i++){
            if(first != return_array[i]){
                flag = 1;
            }
            if(max > return_array[i]){
                max = return_array[i];
                index = i;
            }
        }
        if(flag==1){
            return_array[index] += initial_array[index];
        }
    }
    return return_array[0];
}

/*
Esta funcion retorna el id del proceso con mayor prioridad o con el menor periodo
y retonar -1 sino esta listo para ejecutarse.
*/
int prior_process(struct process_list *list, int lcm, int number_process){
    int prior = -1, less_periode = lcm, i;

    struct process *current = list->first;

    for(i=0; i < number_process; i++){
        //Buscamos proceso con menor periodo, que no se haya ejecutado por completo
        if(current->execution_time != 0){
            if(less_periode > current->period){
                prior = i;
                less_periode = current->period;
            }
        }
        current = current->next;

    }
    return prior;
}

/*
*/
bool utilization_test(struct process_list *list){
    int i;
    bool solution = false;
    float utilization_time, utilization_limit, result, m;
    struct process *current = list->first;

    int number_process = list->elements;

    //Test UB -> Utilization bound(UB)

    //Debemos saber si el sistema es planificable
    //Factor de utilizacion sumatoria hasta m de Ci/Pi, donde m = cantidad de procesos
    //Ci= tiempo de ejecución para la tarea Pi= periodo de la tarea

    //Caculo U
    for(i=0; i < number_process; i++){
        result = current->execution_time / current->period;
        utilization_time += result;
        printf("%s\n", current->name);
        current = current->next;
    }

    //Calcular limite de utilizacion
    // m * (2**(1/m) - 1)
    //Calculo U(m)
    m = (float)number_process;
    m = m * ((pow(2.0,1/m)) - 1);
    utilization_limit = m;

    if(utilization_time < utilization_limit){
        solution = true;
    }

    return solution;
}


