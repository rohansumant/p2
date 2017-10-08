#include "fcfs.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h>


static void findWaitingTime(process *ptr, int *waitingTime)
{
    waitingTime[0] = 0;

    for(int i = 1; i < NUMBER_OF_PROCS; ++i) {
        waitingTime[i] = ptr[i-1].expected_runtime + waitingTime[i-1];
    }
}

static void findTurnAroundTime(process *ptr, int *waitingTime, int *turnAroundTime)
{
    for(int i = 0; i < NUMBER_OF_PROCS; ++i) {
        turnAroundTime[i] = ptr[i].expected_runtime + waitingTime[i];
    }
}

static void printFCFSStats(process *ptr, int *waitingTime, int *turnAroundTime) 
{
    int totalWaitTime = 0;
    int totalTurnAroundTime = 0;

    printf("Process id \t Burst Time \t Waiting time \t Turn around time \t\n");

    for(int i = 0; i < NUMBER_OF_PROCS; ++i) {
        totalWaitTime += waitingTime[i];
        totalTurnAroundTime += turnAroundTime[i];

        printf("%d\t\t %.1f\t\t %d\t\t %d\t\t\n", ptr[i].pid, ptr[i].expected_runtime, waitingTime[i], turnAroundTime[i]);
    }

    printf("Average waiting time %f\n", (float)totalWaitTime/(float)NUMBER_OF_PROCS);
    printf("Average turn around time %f\n", (float)totalTurnAroundTime/(float)NUMBER_OF_PROCS);
}

static void findAverageTime(process *ptr)
{
    int waitingTime[NUMBER_OF_PROCS];
    int turnAroundTime[NUMBER_OF_PROCS];

    findWaitingTime(ptr, waitingTime);

    findTurnAroundTime(ptr, waitingTime, turnAroundTime);

    printFCFSStats(ptr, waitingTime, turnAroundTime);
}

// First Come First Served (non-preemptive)
void fcfs(process *ptr)
{
// sort on arrival time
    int i;
    qsort(ptr, NUMBER_OF_PROCS, sizeof(process), compare_arrival_times);
    
    for(i=0; i<NUMBER_OF_PROCS; i++)
    {
        printf("pid: %d , arrival time: %.1f\t expected_runtime :%.1f\n", ptr[i].pid, ptr[i].arrival_time, ptr[i].expected_runtime);
    }
    
    findAverageTime(ptr);

    // for (i=0; i<QUANTA; i++)
    // {
    //     printf("Q%d:\n", i);
    // }
}
