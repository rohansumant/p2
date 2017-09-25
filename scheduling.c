#include <stdlib.h>
#include <stdio.h>
#include "priority_queue.h"

#define NUMBER_OF_PROCS 10
#define RUNS_PER_ALGO 5
#define QUANTA 100

// TODO: Answer the following:
// can we add remaining time to the process struct?
// why does arrival time need to be a float?
// why does expected run time run from 0.1 to 10
//     with possible values: [ 0.1, 1, 2, 3, 4 .. 10 ]


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

int cmp_arrival_time_expected_runtime(const void* p1, const void* p2) {
    int at1 = ((process*)p1)->arrival_time, at2 = ((process*)p2)->arrival_time;
    if(at1 == at2) return ((process*)p1)->expected_runtime < ((process*)p2)->expected_runtime;
    return at1 < at2;
}

int sjf(process *arr) {
    qsort(arr, NUMBER_OF_PROCS, sizeof(process), cmp_arrival_time_expected_runtime);
    int time[QUANTA+15];
    int curr_time = 0;
    for(int i=0;i<NUMBER_OF_PROCS;i++) {
	for(int j=curr_time;j<curr_time+arr[i].expected_runtime;j++) time[j] = arr[i].pid;
	curr_time += arr[i].expected_runtime;
    }
    for(int i=0;i<curr_time;i++) {
	printf("Current time quanta:%d PID:%d\n",i,time[i]);
    }
    return 0;
}

// Shortest Remaining Time (preemptive)

float key_expected_runtime(process p) {
    return p.expected_runtime;
}

int srt(process *arr) {
    qsort(arr, NUMBER_OF_PROCS, sizeof(process), compare_arrival_times);
    int ptr = 0;
    int i = 0;
    priority_queue Q;
    init(&Q);
    while(ptr != NUMBER_OF_PROCS || size(&Q) != 0) {
	while(ptr != NUMBER_OF_PROCS && arr[ptr].arrival_time == i) {
	    push(&Q,arr[ptr],key_expected_runtime);
	    ptr++;
	}
	if(size(&Q)) {
	    process p = peek(&Q);
	    printf("Current time quanta:%d PID:%d\n",i,p.pid);
	    pop(&Q,key_expected_runtime);
	    if(p.expected_runtime > 1.0) {
		p.expected_runtime -= 1.0;
		push(&Q,p,key_expected_runtime);
	    }
	}
	i++;
    }
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
/*
    printf("Running fcfs %d times\n", RUNS_PER_ALGO);
    for (i=0; i< RUNS_PER_ALGO; i++)
    {
        printf("%d\n", i);
        generate_procs(buff);
        fcfs(buff);
    }
*/
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
	for(int j=0;j<NUMBER_OF_PROCS;j++) {
	    printf("%d %d %f\n",buff[j].pid,buff[j].arrival_time,buff[j].expected_runtime);
	}
        srt(buff);
    }
/*
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
*/
}
