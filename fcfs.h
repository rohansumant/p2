#ifndef FCFS_H
#define FCFS_H

#include "common.h"

void fcfs(process *);

static void findWaitingTime(process *, int *);
static void findTurnAroundTime(process *, int *, int *);
static void printFCFSStats(process *, int *, int *);
static void findAverageTime(process *);

#endif