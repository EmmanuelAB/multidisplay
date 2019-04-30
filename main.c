#include "scheduling.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));


    struct process_list *list_lottery;
    list_lottery = malloc(sizeof(struct process_list));

    add_element(list_lottery,"Word",10,0);
    add_element(list_lottery,"Paint",20,0);
    add_element(list_lottery,"Photoshop",3,0);
    add_element(list_lottery,"PowerPoint",6,0);
    add_element(list_lottery,"Excel",1,0);

    struct process_list *list_rr = malloc(sizeof(struct process_list));

    add_element(list_rr,"Atom",10,0);
    add_element(list_rr,"CodeBlocks",4,0);
    add_element(list_rr,"NetBeans",8,0);
    add_element(list_rr,"Dev C++",5,0);
    add_element(list_rr,"SublimeText",12,0);

    struct process_list *list_rt = malloc(sizeof(struct process_list));

    add_element(list_rt,"Ubuntu",10,30);
    add_element(list_rt,"Debian",15,40);
    add_element(list_rt,"Kali Linux",5,50);

    //round_robin(list_rr,QUANTUM);

    real_time(list_rt);

    //scheduling_lottery(list_lottery);

    return 0;
}
