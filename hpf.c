#include "hpf.h"
#include <assert.h>

void Print_HPF_Stats(process* ptr,int* fs,int* ls,int* done) {
    float avg_response_time = 0,
	  avg_turn_around_time = 0,
	  throughput = 0,
	  avg_waiting_time = 0;
    int processes = 0;


    float avg_rt[4] = {0}, avg_tat[4] = {0}, avg_wt[4] = {0}, th[4] = {0};
    int proc_cnt[4] = {0};

    for(int i=0;i<4;i++) {
	avg_rt[i] = 0; avg_tat[i] = 0;
	avg_wt[i] = 0; th[i] = 0;
	proc_cnt[i] = 0;
    }

    for(int i=0;i<NUMBER_OF_PROCS;i++) if(done[ptr[i].pid]) {
	processes++;
	avg_response_time += fs[ptr[i].pid] - ptr[i].arrival_time + 1;
	avg_turn_around_time += ls[ptr[i].pid] - ptr[i].arrival_time + 1;
	assert((ls[ptr[i].pid] - ptr[i].arrival_time + 1) >= ptr[i].expected_runtime);
	avg_waiting_time += (ls[ptr[i].pid] - ptr[i].arrival_time + 1) - ptr[i].expected_runtime;
	
	//Priority specific statistics collection
	
	int ix = ptr[i].priority - 1; // Get the array index
	proc_cnt[ix]++;
	avg_rt[ix] += fs[ptr[i].pid] - ptr[i].arrival_time + 1;
	avg_tat[ix] += ls[ptr[i].pid] - ptr[i].arrival_time + 1;
	avg_wt[ix] += (ls[ptr[i].pid] - ptr[i].arrival_time + 1) - ptr[i].expected_runtime;
    }
    


    for(int i=0;i<4;i++) { //Priority specific statistics calculation
	avg_rt[i] /= proc_cnt[i];
	avg_tat[i] /= proc_cnt[i];
	avg_wt[i] /= proc_cnt[i];
	th[i] = (100.0*proc_cnt[i]) / QUANTA;
    }

    printf("\n***Overall statistics***\n");
    printf("Total completed processes: %d\n",processes);
    avg_response_time /= processes;
    avg_turn_around_time /= processes;
    avg_waiting_time /= processes;
    printf("Avg. Response time: %f\nAvg. Turn around-time: %f\nAvg. Waiting time: %f\n",avg_response_time,avg_turn_around_time,avg_waiting_time);
    throughput = (100.0*processes)/QUANTA;
    printf("Avg. throughput: %f\n",throughput);
    
    // Display priority specific stats
    
    printf("\n***Priority specific statistics***\n");
    
    for(int i=0;i<4;i++) {
	printf("Priority %d:\n",i+1);
	printf("Avg. Response time: %f\nAvg. Turn around-time: %f\nAvg. Waiting time: %f\n",avg_rt[i],avg_tat[i],avg_wt[i]);
	printf("Avg. throughput: %f\n",th[i]);
    }
}
	


int hpf_preemptive(process *ptr) {
    qsort(ptr, NUMBER_OF_PROCS, sizeof(process), compare_arrival_times);
    process Q[4][NUMBER_OF_PROCS];
    int ix[4] = {0,0,0,0}; //index of free position in the the priority queue
    int prev[4] = {-1,-1,-1,-1}; // index of the process which was last served

    int first_touch[NUMBER_OF_PROCS]; // Store information about when the process received CPU for the first time
    int last_touch[NUMBER_OF_PROCS]; // Store information about ^....................................^ last time
    int done[NUMBER_OF_PROCS];

    // Initialize arrays
    for(int i=0;i<NUMBER_OF_PROCS;i++) {
	first_touch[i] = -1;
	last_touch[i] = -1;
	done[i] = 0;
    }

    int x = 0; // pointer to the beginning of the process queue (ptr);
    for(int q=0;q<QUANTA;q++) {
	// First, look for any new processes starting at this quantum
	// and populate the queue accordingly
	while(x < NUMBER_OF_PROCS && ptr[x].arrival_time <= q) {
	    int pr = ptr[x].priority - 1;
	    Q[pr][ix[pr]] = ptr[x];
	    ix[pr]++;
	    x++;
	}
	// check process from which queue is to be alloted this quantum
	int target_queue = -1;
	for(int i=0;i<4;i++) {
	    for(int j=0;j<ix[i];j++) if(Q[i][j].expected_runtime > 0.0) { // if expected_runtime <= 0.0 then the process has already completed
		target_queue = i;
		break;
	    }
	    if(target_queue != -1) break;
	}
	if(target_queue == -1) {
	    printf("No jobs at quantum %d\n",q);
	    continue;
	}
	int target_process_ix;
	if(prev[target_queue] == -1) target_process_ix = 0;
	else {
	    assert(ix[target_queue] != 0);
	    int i = (prev[target_queue]+1) % ix[target_queue];
	    while(Q[target_queue][i].expected_runtime <= 0) {
		i = (i+1)%ix[target_queue];
	    }
	    target_process_ix = i;
	}

	process* target_process = &(Q[target_queue][target_process_ix]);

	if(first_touch[target_process->pid] == -1) first_touch[target_process->pid] = q;
	last_touch[target_process->pid] = q;
	if(target_process->expected_runtime < 1.0) {
	    printf("Q %d: %d(%f,%d)   (CPU idle for %f quantum)\n",q,target_process->pid,target_process->expected_runtime,target_process->priority,(1.0-(target_process->expected_runtime)));
	} else {
	    printf("Q %d: %d(%f,%d)\n",q,target_process->pid,target_process->expected_runtime,target_process->priority);
	}
	target_process->expected_runtime -= 1.0;
	if(target_process->expected_runtime <= 0) {
	   done[target_process->pid] = 1;
	} 
	prev[target_queue] = target_process_ix;
    }
    Print_HPF_Stats(ptr,first_touch,last_touch,done);
    return 0;
}

int hpf_nonpreemptive(process *ptr) {
    qsort(ptr, NUMBER_OF_PROCS, sizeof(process), compare_arrival_times);
    process Q[4][NUMBER_OF_PROCS];

    int ix[4] = {0,0,0,0}; //index of free position in the the priority queue
    int first_touch[NUMBER_OF_PROCS]; // Store information about when the process received CPU for the first time
    int last_touch[NUMBER_OF_PROCS]; // Store information about ^....................................^ last time
    int done[NUMBER_OF_PROCS];

    // Initialize arrays
    for(int i=0;i<NUMBER_OF_PROCS;i++) {
	first_touch[i] = -1;
	last_touch[i] = -1;
	done[i] = 0;
    }
    
    process* target_process = NULL;

    int x = 0; // pointer to the beginning of the process queue (ptr);
    for(int q=0;q<QUANTA;q++) {
	// First, look for any new processes starting at this quantum
	// and populate the queue accordingly
	while(x < NUMBER_OF_PROCS && ptr[x].arrival_time <= q) {
	    int pr = ptr[x].priority - 1;
	    Q[pr][ix[pr]] = ptr[x];
	    ix[pr]++;
	    x++;
	}


	if(!target_process) {
	    // check process from which queue is to be alloted this quantum
	    int target_queue = -1;
	    for(int i=0;i<4;i++) {
		for(int j=0;j<ix[i];j++) if(Q[i][j].expected_runtime > 0.0) { // if expected_runtime <= 0.0 then the process has already completed
		    target_queue = i;
		    break;
		}
		if(target_queue != -1) break;
	    }
	    if(target_queue == -1) {
		printf("No jobs at quantum %d\n",q);
		continue;
	    }
	    int target_process_ix = 0;
	    while(target_process_ix < ix[target_queue] && Q[target_queue][target_process_ix].expected_runtime <= 0) {
		target_process_ix++;
	    }
	    target_process = &(Q[target_queue][target_process_ix]);
	} 

	if(first_touch[target_process->pid] == -1) first_touch[target_process->pid] = q;
	last_touch[target_process->pid] = q;

	if(target_process->expected_runtime < 1.0) {
	    printf("Q %d: %d(%f,%d)   (CPU idle for %f quantum)\n",q,target_process->pid,target_process->expected_runtime,target_process->priority,(1.0-(target_process->expected_runtime)));
	} else {
	    printf("Q %d: %d(%f,%d)\n",q,target_process->pid,target_process->expected_runtime,target_process->priority);
	}

	target_process->expected_runtime -= 1.0;
	if(target_process->expected_runtime <= 0) {
	   done[target_process->pid] = 1;
	   target_process = NULL;
	}
    }
    Print_HPF_Stats(ptr,first_touch,last_touch,done);
    return 0;
}

