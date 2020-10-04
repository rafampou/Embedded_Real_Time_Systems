#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <unistd.h>

#include <stdio.h>
#include <pthread.h>
#include <sys/time.h> //gettimeofday()

typedef struct timer{
    unsigned int Period, TaskToExecute, StartDelay;
    void *(*StartFcn)(void *), *(*StopFcn)(void *), *(*TimerFcn)(void *), *(*ErrorFcn)(void *);
    void *UserData;
    int id;
    struct timeval startTime;
} Timer;


pthread_t *start(Timer *t);
pthread_t *startat(Timer *t,unsigned int y,unsigned int m,unsigned int d,unsigned int h,unsigned int min,unsigned int sec);

void timerInit(Timer *t,unsigned int Period,unsigned int TaskToExecute,unsigned int  StartDelay,
               void *(*StartFcn)(void *),void *(*StopFcn)(void *),void *(*TimerFcn)(void *),void *(*ErrorFcn)(void *),
               void *UserData);

#endif // TIMER_H_INCLUDED
