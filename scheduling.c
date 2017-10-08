#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "fcfs.h"

// TODO: Answer the following:
// can we add remaining time to the process struct?
// why does arrival time need to be a float?
// why does expected run time run from 0.1 to 10
//     with possible values: [ 0.1, 1, 2, 3, 4 .. 10 ]

// Shortest Job First (non-preemptive)
int sjf(process *ptr)
{
// sort on expected runtimes
    int i;
    qsort(ptr, NUMBER_OF_PROCS, sizeof(process), compare_expected_runtimes);
    for(i=0; i<NUMBER_OF_PROCS; i++)
    {
        printf("pid: %d , expected runtime: %f\n", ptr[i].pid, ptr[i].expected_runtime);
    }

    return 0;
}

// Shortest Remaining Time (preemptive)
int srt(process *ptr)
{
// sort on shortest remaining time
    int i;
    qsort(ptr, NUMBER_OF_PROCS, sizeof(process), compare_expected_runtimes);
    for(i=0; i<NUMBER_OF_PROCS; i++)
    {
        printf("pid: %d , expected runtime: %f\n", ptr[i].pid, ptr[i].expected_runtime);
    }

    return 0;
}

// Round Robin (preemptive)
int rr(process *ptr)
{
// dont sort
    int i;
    for(i=0; i<NUMBER_OF_PROCS; i++)
    {
        printf("pid: %d\n", ptr[i].pid);
    }

    return 0;
}

// Highest Priority First (both preemptive and non-preemptive)
int hpf(process *ptr)
{
// sort on priorities
    int i;
    qsort(ptr, NUMBER_OF_PROCS, sizeof(process), compare_priorities);
    for(i=0; i<NUMBER_OF_PROCS; i++)
    {
        printf("pid: %d , priority: %d\n", ptr[i].pid, ptr[i].priority);
    }

    return 0;
}


int main()
{
    printf("hello main\n");
    srand(0);
    process proc_list[NUMBER_OF_PROCS];
    int i;
    process *buff = malloc(NUMBER_OF_PROCS * sizeof(process));

    printf("Running fcfs %d times\n", RUNS_PER_ALGO);
    for (i=0; i< RUNS_PER_ALGO; i++)
    {
        printf("run #%d:\n", i);
        generate_procs(buff);
        print_procs(buff);
        fcfs(buff);
    }

//    printf("Running sjf %d times\n", RUNS_PER_ALGO);
//    for (i=0; i< RUNS_PER_ALGO; i++)
//    {
//        printf("%d\n", i);
//        generate_procs(buff);
//        sjf(buff);
//    }
//
//    printf("Running srt %d times\n", RUNS_PER_ALGO);
//    for (i=0; i< RUNS_PER_ALGO; i++)
//    {
//        printf("%d\n", i);
//        generate_procs(buff);
//        srt(buff);
//    }
//
//    printf("Running rr %d times\n", RUNS_PER_ALGO);
//    for (i=0; i< RUNS_PER_ALGO; i++)
//    {
//        printf("%d\n", i);
//        generate_procs(buff);
//        rr(buff);
//    }
//
//    printf("Running hpf %d times\n", RUNS_PER_ALGO);
//    for (i=0; i< RUNS_PER_ALGO; i++)
//    {
//        printf("%d\n", i);
//        generate_procs(buff);
//        hpf(buff);
//    }

    return 0;
}
