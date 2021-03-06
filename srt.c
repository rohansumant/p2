#include "srt.h"


// takes a sorted process array and a quanta and returns the next valid proc to run or NULL if none found
process * next_proc_pre(process* ptr, int Q){
    int i =0;
    for (i=0; i< NUMBER_OF_PROCS; i++){
        if (Q >= ptr[i].arrival_time && ptr[i].remaining_runtime > 0) return &ptr[i];
    }
    return NULL;
}

// Shortest Remaining Time (preemptive)
int srt(process *ptr)
{
    int i;
    int done_procs = 0;
    float idle_time = 0;
    float q_idle_time = 0;
    process* c_proc = NULL;

    // sort on shortest remaining time
    qsort(ptr, NUMBER_OF_PROCS, sizeof(process), compare_remaining_runtimes);

    printf("expected order:\n");
    print_procs(ptr);

    printf("\n Simulation starting...\n\n");
    for (i=0; i<QUANTA; i++)
    {
        // sort on remaining time
        qsort(ptr, NUMBER_OF_PROCS, sizeof(process), compare_remaining_runtimes);
        q_idle_time = 0;    // var to measure idle time in this quanta
        if (done_procs == NUMBER_OF_PROCS) break; // if all procs are finished we break this loop

        c_proc = next_proc_pre(ptr, i);

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
        printf("| QUANTA %02d:  I D L E  |\n",i);
        idle_time += 1.0;
    }

    printf("\n\n%d processes did not finish after %d QUANTA\n", NUMBER_OF_PROCS - done_procs, QUANTA);
    for (i=0; i< NUMBER_OF_PROCS; i++){
        if (ptr[i].remaining_runtime > 0){
            printf("pid:%d remaining time: %f\n", ptr[i].pid, ptr[i].remaining_runtime);
        }
    }

    printf("\ncpu was idle %3.1f Quanta \n", idle_time);

    return 0;
}
