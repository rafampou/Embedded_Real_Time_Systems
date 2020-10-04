#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#define QUEUESIZE 3

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "timer.h"

extern FILE *fp;

typedef struct{
    void *(*jobFcn)(void *);
    void *data;
} job;


typedef struct {
  job *buf[QUEUESIZE];
  long head, tail;
  int full, empty;
  pthread_mutex_t *mut;
  pthread_cond_t *notFull, *notEmpty;
} queue;


queue *queueInit (void);
void queueDelete (void);
void queueAdd (job *);
job *queueDel();

void *consumer(void* arg);

#endif // QUEUE_H_INCLUDED
