#include "common.h"

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
