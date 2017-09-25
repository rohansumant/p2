#define  _PRIORITY_QUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduling.h"

typedef struct {
    process *arr;
    int n;
} priority_queue;


void swap(const void* a,const void* b,const int size) {
    void *ptr = malloc(size);
    memcpy(ptr,a,size);
    memcpy(a,b,size);
    memcpy(b,ptr,size);
    free(ptr);
}

void init(priority_queue* q) {
    if(!q) {
	printf("Priority queue not initialized\n");
	return;
    } else {
	q->arr = malloc(sizeof(process)*100);
	q->n = 0;
    }
}

void push(priority_queue* q,process p,float (*key)(process)) {
    int _n = q->n;
    q->arr[_n] = p;
    while(_n) {
    	int par = _n&1 ? _n/2:_n/2 - 1;
	if(key(q->arr[_n]) < key(q->arr[par])) {
	    swap(&q->arr[_n],&q->arr[par],sizeof(process));
	    _n = par;
	} else break;
    }
    q->n++;
}

process peek(priority_queue* q) {
    return q->arr[0];
}

void go(priority_queue* q,int ix,float (*key)(process)) {
    float val = key(q->arr[ix]);
    float lval = ix*2+1 >= q->n ? 1e18 : key(q->arr[ix*2+1]);
    float rval = ix*2+2 >= q->n ? 1e18 : key(q->arr[ix*2+2]);
    if(val <= lval && val <= rval) return;
    if(lval < rval) {
	swap(&(q->arr[ix]),&(q->arr[ix*2+1]),sizeof(process));
	go(q,ix*2+1,key);
    } else {
	swap(&(q->arr[ix]),&(q->arr[ix*2+2]),sizeof(process));
	go(q,ix*2+2,key);
    }
}

void pop(priority_queue* q,float (*key)(process)) {
    int x = q->n-1;
    swap(&q->arr[0],&q->arr[x],sizeof(process));
    q->n--;
    go(q,0,key);
}

void _debug(priority_queue* q,float (*key)(process)) {
    for(int i=0;i<q->n;i++) {
	printf("%d  %f\n",i,key(q->arr[i]));
    }
}

int size(priority_queue *q) {
    if(q) return q->n;
    return -1;
}

/*
int main() {
    srand(time(0));
    priority_queue Q;
    init(&Q);
    for(int i=0;i<10;i++) {
	float val = 1.0 * (rand()%50);
	printf("Pushing %f \n",val);
	push(&Q,val);
    }
    _debug(&Q);
    while(Q.n) {
	float val = peek(&Q);
	pop(&Q);
	printf("Popping %f \n",val);
    }
    return 0;
}
*/
