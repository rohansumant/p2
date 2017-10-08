#include "sjf.h"



// Shortest Job First (non-preemptive)
int sjf(process *ptr)
{
// sort on expected runtimes
    int i;
    int idx = 0 ;
    int done_procs = 0;
    float idle_time = 0;
    float q_idle_time = 0;
    qsort(ptr, NUMBER_OF_PROCS, sizeof(process), compare_expected_runtimes);

    printf("expected order:\n");
    print_procs(ptr);

    printf("\n Simulation starting...\n\n");
    for (i=0; i<QUANTA; i++)
    {
        q_idle_time = 0;    // var to measure idle time in this quanta

        if (done_procs == NUMBER_OF_PROCS) break; // if all procs are finished we break this loop

        // Pick the next proc that has some remaining time
        while (ptr[idx].remaining_runtime <= 0){
            idx++;
            idx = idx % NUMBER_OF_PROCS;
        }

        printf("| QUANTA %02d: ", i);  // QUANTA header

        // Now we found a process to run and we process by subtracting 1 from the remaining time
        ptr[idx].remaining_runtime -= 1;

        // Check to see if this process finished, if so, we record the cpu idle time in this quanta
        if (ptr[idx].remaining_runtime <= 0){
            done_procs++;
            q_idle_time = (-1) * ptr[idx].remaining_runtime;
            ptr[idx].remaining_runtime = 0;
        }

        // Print the process id and its remaining runtime
        printf("[p%02d](%2.1f)|", ptr[idx].pid, ptr[idx].remaining_runtime);

        // If the cpu was idle in this quanta print that and add it to the total idle time
        if (q_idle_time){
            printf("<< CPU IDLE for %2.1f QUANTA |", q_idle_time);
            idle_time += q_idle_time;
        }
        printf("\n");

        // Advance our index variable
        idx++;
        idx = idx % NUMBER_OF_PROCS;
    }

    // Finish the rest of the QUANTA
    for (i=i; i< QUANTA; i++){
        printf("| QUANTA %02d: IDLE  |\n",i);
        idle_time += 1.0;
    }

    printf("\ncpu was idle %3.1f Quanta \n", idle_time);


    return 0;
}
