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
// void fcfs(process *ptr)
// {
// // sort on arrival time
//     int i;
//     qsort(ptr, NUMBER_OF_PROCS, sizeof(process), compare_arrival_times);
    
//     for(i=0; i<NUMBER_OF_PROCS; i++)
//     {
//         printf("pid: %d , arrival time: %.1f\t expected_runtime :%.1f\n", ptr[i].pid, ptr[i].arrival_time, ptr[i].expected_runtime);
//     }
    
//     findAverageTime(ptr);

//     // for (i=0; i<QUANTA; i++)
//     // {
//     //     printf("Q%d:\n", i);
//     // }
// }

// takes a sorted process array and a quanta and returns the next valid proc to run or NULL if none found
process * next_proc_fcfs(process* ptr, int Q, process* c){
    int i =0;
    if (c != NULL && c->remaining_runtime > 0) return c; // non-preemptive-ness
    for (i=0; i< NUMBER_OF_PROCS; i++){
        if (Q >= ptr[i].arrival_time && ptr[i].remaining_runtime > 0) return &ptr[i];
    }
    return NULL;
}

void fcfs(process *ptr)
{
	int i;
    int done_procs = 0;
    float idle_time = 0;
    float q_idle_time = 0;
    process* c_proc = NULL;

    // sort on expected runtime
    qsort(ptr, NUMBER_OF_PROCS, sizeof(process), compare_arrival_times);

    printf("expected order:\n");
    print_procs(ptr);

    printf("\n Simulation starting...\n\n");
    for (i=0; i<QUANTA; i++)
    {
    	q_idle_time = 0;    // var to measure idle time in this quanta

        if (done_procs == NUMBER_OF_PROCS) break; // if all procs are finished we break this loop

        c_proc = next_proc_fcfs(ptr, i, c_proc);

        // If c_proc is NULL there are no available procs to run so print QUANTA header with idle time and continue.
        if (c_proc == NULL){
            printf("| QUANTA %02d:  I D L E  |\n",i);
            idle_time += 1.0;
            continue;
        }

        // If we get here, we have a valid proc to run at c_proc
        printf("| QUANTA %02d: ", i);  // QUANTA header

        // Now we found a process to run and we process by subtracting 1 from the remaining time
        c_proc->remaining_runtime -= 1;

        // Check to see if this process finished, if so, we record the cpu idle time in this quanta
        if (c_proc->remaining_runtime <= 0){
            done_procs++;
            q_idle_time = (-1) * c_proc->remaining_runtime;
            c_proc->remaining_runtime = 0;
        }

        // Print the process id and its remaining runtime
        printf("[p%02d](%2.1f)|", c_proc->pid, c_proc->remaining_runtime);

        // If the cpu was idle in this quanta print that and add it to the total idle time
        if (q_idle_time){
            printf("<< CPU IDLE for %2.1f QUANTA |", q_idle_time);
            idle_time += q_idle_time;
        }
        printf("\n");
    }

        // Finish the rest of the QUANTA
    for (i=i; i< QUANTA; i++){
        printf("| QUANTA %02d: IDLE  |\n",i);
        idle_time += 1.0;
    }

    printf("\n\n%d processes did not finish after %d QUANTA\n", NUMBER_OF_PROCS - done_procs, QUANTA);
    for (i=0; i< NUMBER_OF_PROCS; i++){
        if (ptr[i].remaining_runtime > 0){
            printf("pid:%d remaining time: %f\n", ptr[i].pid, ptr[i].remaining_runtime);
        }
    }

    printf("\ncpu was idle %3.1f Quanta \n", idle_time);

}