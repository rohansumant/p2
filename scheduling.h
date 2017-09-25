#define _SCHEDULING_H_


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


