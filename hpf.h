#ifndef _HPF_H
#define _HPF_H

#include "common.h"

int hpf_preemptive(process*);
int hpf_nonpreemptive(process*);
void Print_HPF_Stats(process*,int*,int*,int*);


#endif
