#include "common.h"
#include <stdlib.h>
#include <stdio.h>

int compare_arrival_times(const void * a, const void * b)
{
    process *p1 = (process*)a;
    process *p2 = (process*)b;
    return (p1->arrival_time - p2->arrival_time);
}

int compare_expected_runtimes(const void * a, const void * b)
{
    process *p1 = (process*)a;
    process *p2 = (process*)b;
    float r = p1->expected_runtime - p2->expected_runtime;
    if (r>0) return 1;
    if (r<0) return -1;
    return r;
}

int compare_remaining_runtimes(const void * a, const void * b)
{
    process *p1 = (process*)a;
    process *p2 = (process*)b;
    float r = p1->remaining_runtime - p2->remaining_runtime;
    if (r>0) return 1;
    if (r<0) return -1;
    return r;
}

int compare_priorities(const void * a, const void * b)
{
    process *p1 = (process*)a;
    process *p2 = (process*)b;
    return (p1->priority - p2->priority);
}


int generate_procs(process *p)
{
    int pr;
    int i;
    float service_time;
    float tmp;
    for(i=0; i<NUMBER_OF_PROCS; i++)
    {
        p[i].pid = i;

        tmp = rand() % 1000;
        tmp = tmp / 10;
        p[i].arrival_time = tmp;

        tmp = rand() % 101;
        tmp = tmp / 10;
        if (tmp < 1) tmp += 1;
        p[i].expected_runtime = tmp;
        p[i].remaining_runtime = tmp;

        pr = rand() % 5;
        if (pr == 0) pr += 1;
        p[i].priority = pr;
    }

    return 0;
}

int print_procs(process *b){
    int i;
    printf("________________________________________________________\n");
    printf("|   pid    |   a time |   e time |   r time |  priority|\n");
    printf("|======================================================|\n");
    for (i=0; i<NUMBER_OF_PROCS; i++){

        printf("|   %-7d|   %-7.1f|   %-7.1f|   %-7.1f|   %-7d|\n", \
            b[i].pid,\
            b[i].arrival_time,\
            b[i].expected_runtime,\
            b[i].remaining_runtime,\
            b[i].priority);

    }
    printf("========================================================\n");

    return 0;
}

//// takes a sorted process array and a quanta and returns the next valid proc to run or NULL if none found
//process * next_proc(process* ptr, int Q){
//    int i =0;
//    for (i=0; i< NUMBER_OF_PROCS; i++){
//        if (Q > ptr[i].arrival_time && ptr[i].remaining_runtime > 0) return &ptr[i];
//    }
//    return NULL;
//}
