#ifndef COMMON_H
#define COMMON_H

#define NUMBER_OF_PROCS 10
#define RUNS_PER_ALGO 5
#define QUANTA 100

typedef struct {
    int pid;
    int arrival_time;
    float expected_runtime;
    int priority;
} process;

int compare_arrival_times(const void *, const void *);
int compare_expected_runtimes(const void *, const void *);
int compare_priorities(const void *, const void *);

#endif