#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>

#define NUMBER_OF_PROCS 10
#define RUNS_PER_ALGO 5
#define QUANTA 100

typedef struct {
    int pid;
    float arrival_time;
    float expected_runtime;
    float remaining_runtime;
    int priority;
    int age;
} process;

int compare_arrival_times(const void *, const void *);
int compare_expected_runtimes(const void *, const void *);
int compare_remaining_runtimes(const void *, const void *);
int compare_priorities(const void *, const void *);
int generate_procs(process *);
int print_procs(process *);
//process * next_proc(process*, int);

#endif
