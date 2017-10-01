#include <stdlib.h>
#include <stdio.h>


#define NUMBER_OF_PROCS 10
#define RUNS_PER_ALGO 5
#define QUANTA 100

// TODO: Answer the following:
// can we add remaining time to the process struct?
// why does arrival time need to be a float?
// why does expected run time run from 0.1 to 10
//     with possible values: [ 0.1, 1, 2, 3, 4 .. 10 ]

typedef struct {
    float arrival_time;
    float expected_runtime;
    int priority;
} processf;


typedef struct {
    int pid;
    int arrival_time;
    float expected_runtime;
    int priority;
} process;


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

int compare_priorities(const void * a, const void * b)
{
    process *p1 = (process*)a;
    process *p2 = (process*)b;
    return (p1->priority - p2->priority);
}

// First Come First Served (non-preemptive)
int fcfs(process *ptr)
{
// sort on arrival time
    int i;
    qsort(ptr, NUMBER_OF_PROCS, sizeof(process), compare_arrival_times);
    for(i=0; i<NUMBER_OF_PROCS; i++)
    {
        printf("pid: %d , arrival time: %d\n", ptr[i].pid, ptr[i].arrival_time);
    }
    for (i=0; i<QUANTA; i++)
    {
        printf("Q%d:\n", i);
    }

    return 0;
}

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

int generate_procs(process *p)
{
    int pr;
    int i;
    float service_time;
    for(i=0; i<NUMBER_OF_PROCS; i++)
        {
            p[i].pid = i;
            p[i].arrival_time = rand() % 100;
            service_time = rand() % 11;
            if (service_time == 0) service_time += 0.1;
            p[i].expected_runtime = service_time;
            pr = rand() % 5;
            if (pr == 0) pr += 1;
            p[i].priority = pr;
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
        printf("%d\n", i);
        generate_procs(buff);
        fcfs(buff);
    }

    printf("Running sjf %d times\n", RUNS_PER_ALGO);
    for (i=0; i< RUNS_PER_ALGO; i++)
    {
        printf("%d\n", i);
        generate_procs(buff);
        sjf(buff);
    }

    printf("Running srt %d times\n", RUNS_PER_ALGO);
    for (i=0; i< RUNS_PER_ALGO; i++)
    {
        printf("%d\n", i);
        generate_procs(buff);
        srt(buff);
    }

    printf("Running rr %d times\n", RUNS_PER_ALGO);
    for (i=0; i< RUNS_PER_ALGO; i++)
    {
        printf("%d\n", i);
        generate_procs(buff);
        rr(buff);
    }

    printf("Running hpf %d times\n", RUNS_PER_ALGO);
    for (i=0; i< RUNS_PER_ALGO; i++)
    {
        printf("%d\n", i);
        generate_procs(buff);
        hpf(buff);
    }

}