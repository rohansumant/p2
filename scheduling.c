#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

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
	while(x < NUMBER_OF_PROCS && ptr[x].arrival_time == q) {
	    int pr = ptr[x].priority - 1;
	    Q[pr][ix[pr]] = ptr[x];
	    ix[pr]++;
	    x++;
	}
	// check process from which queue is to be alloted this quantum
	int target_queue = -1;
	for(int i=0;i<4;i++) {
	    for(int j=0;j<ix[i];j++) if(Q[i][j].expected_runtime > 0) { // if expected_runtime <= 0.0 then the process has already completed
		target_queue = i;
		break;
	    }
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
	    printf("Q %d: %d   (CPU idle for %f quantum)\n",q,target_process->pid,(1.0-(target_process->expected_runtime)));
	} else {
	    printf("Q %d: %d\n",q,target_process->pid);
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
    
    int x = 0; // pointer to the beginning of the process queue (ptr);
    for(int q=0;q<QUANTA;q++) {
	// First, look for any new processes starting at this quantum
	// and populate the queue accordingly
	while(x < NUMBER_OF_PROCS && ptr[x].arrival_time == q) {
	    int pr = ptr[x].priority - 1;
	    Q[pr][ix[pr]] = ptr[x];
	    ix[pr]++;
	    x++;
	}
	// check process from which queue is to be alloted this quantum
	int target_queue = -1;
	for(int i=0;i<4;i++) {
	    for(int j=0;j<ix[i];j++) if(Q[i][j].expected_runtime > 0) { // if expected_runtime <= 0.0 then the process has already completed
		target_queue = i;
		break;
	    }
	}
	if(target_queue == -1) {
	    printf("No jobs at quantum %d\n",q);
	    continue;
	}
	int target_process_ix = 0;
	while(target_process_ix < ix[target_queue] && Q[target_queue][target_process_ix].expected_runtime <= 0) {
	    target_process_ix++;
	}

	process* target_process = &(Q[target_queue][target_process_ix]);

	if(first_touch[target_process->pid] == -1) first_touch[target_process->pid] = q;
	last_touch[target_process->pid] = q;
	if(target_process->expected_runtime < 1.0) {
	    printf("Q %d: %d   (CPU idle for %f quantum)\n",q,target_process->pid,(1.0-(target_process->expected_runtime)));
	} else {
	    printf("Q %d: %d\n",q,target_process->pid);
	}
	target_process->expected_runtime -= 1.0;
	if(target_process->expected_runtime <= 0) {
	   done[target_process->pid] = 1;
	} 
    }
    Print_HPF_Stats(ptr,first_touch,last_touch,done);
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
    srand(time(0));
    //srand(0);
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

    */

    generate_procs(buff);
    process a[NUMBER_OF_PROCS], b[NUMBER_OF_PROCS];
    for(int i=0;i<NUMBER_OF_PROCS;i++) {
	a[i] = buff[i];
	b[i] = buff[i];
    }
    printf("Running hpf_preemptive %d times\n", RUNS_PER_ALGO);
    for (i=0; i< RUNS_PER_ALGO-4; i++)
    {
        printf("%d\n", i);
        //generate_procs(buff);
        hpf_preemptive(a);
    }

    printf("Running hpf_nonpreemptive %d times\n", RUNS_PER_ALGO);
    for (i=0; i< RUNS_PER_ALGO-4; i++)
    {
        printf("%d\n", i);
        //generate_procs(buff);
        hpf_nonpreemptive(b);
    }


}
