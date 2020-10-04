#include "timer.h"
#include "queue.h"
#include <unistd.h>


extern queue *timerQueue;  // The main global queue for timers



void *waitFucntion(void* arg)
{
    Timer *t = (Timer *)arg;
    struct timeval before,after;


    if(t->startTime.tv_sec > 0)
    {
        gettimeofday(&before,NULL);
        int usec_start = (t->startTime.tv_sec - before.tv_sec)*1000;

        //fprintf(fp, "%10d | %9ld | %9ld | StartTimer \n", t->id, before.tv_sec, before.tv_usec);
        //printf("%10d | %9ld | %9ld | StartTimer \n", t->id, before.tv_sec, before.tv_usec);
        if(usec_start >0 )
            usleep( usec_start );
    }


    int period;
    int delaySec;
    int delayUsec;

    while( (t->TaskToExecute)-- > 0)    //Start Period
    {

        gettimeofday(&before,NULL);
        delaySec = before.tv_sec;
        delayUsec = before.tv_usec;

        usleep(10); // delay to read sensor


        job *newjob = (job *)malloc(sizeof(job));
        newjob->jobFcn = (void *(*)(void *))(t->TimerFcn);
        newjob->data = t->UserData;


        //----- Send timer to execution ------//
        pthread_mutex_lock (timerQueue->mut);   //lock fifo mutex
        while (timerQueue->full)      //check if is full
        {
            printf ("\n producer: queue FULL.\n");
            t->ErrorFcn(t);
            pthread_cond_wait (timerQueue->notFull, timerQueue->mut); //wait until is not full
        }
        queueAdd(newjob);
        pthread_mutex_unlock (timerQueue->mut); //un;ock queue
        pthread_cond_signal (timerQueue->notEmpty); //signal not empty
        //----- End execution ------//

        gettimeofday(&after,NULL);

        delaySec = after.tv_sec - delaySec;
        delayUsec = after.tv_usec - delayUsec;
        period = ( delaySec*1000000 + delayUsec + t->Period*1000 );
        //period = t->Period;

      //  fprintf(fp, "%4d %5s |  %9d | %9d | next execution at %d usec\n", t->id,"delay",delaySec,delayUsec, period);
      //  printf( "%4d %5s |  %9d | %9d | next execution at %d usec\n", t->id,"delay",delaySec,delayUsec, period);

        if(period < 0)
        {
            //printf("\n --------- ERROR -------- period < 0\n");
            period = 0;
        }
        fprintf(fp, "%3d |drifting %8d usec\n", t->id,delaySec*1000000+delayUsec);
        usleep(period);

    }//End period
    t->StopFcn(t);

    pthread_exit(t);
    return NULL;
}

pthread_t *start(Timer *t)
{

    struct timeval nowtime;
    gettimeofday(&nowtime,NULL);

    t->StartFcn(t);
    t->startTime.tv_sec = nowtime.tv_sec + t->StartDelay;
    t->startTime.tv_usec = 0;

  //  fprintf(fp, "%10d | %9ld | %9ld | Create Thread \n", t->id, nowtime.tv_sec, nowtime.tv_usec);
    printf("%10d | %9ld | %9ld | Create Thread \n", t->id, nowtime.tv_sec, nowtime.tv_usec);

    pthread_t *thread = (pthread_t *)malloc(sizeof(pthread_t));
    pthread_create(thread, NULL, waitFucntion,(Timer *)t);

    return thread;
}

pthread_t *startat(Timer *t,unsigned int y,unsigned int m,unsigned int d,unsigned int h,unsigned int min,unsigned int sec)
{
    struct timeval nowtime;
    gettimeofday(&nowtime,NULL);

    t->StartFcn(t);
    struct tm time_info;
	time_info.tm_year = y-1900;
	time_info.tm_mon = m - 1;
	time_info.tm_mday = d;
	time_info.tm_hour = h;
	time_info.tm_min = min;
	time_info.tm_sec = sec +t->StartDelay;
	time_info.tm_isdst = 0;

    t->startTime.tv_sec = mktime(&time_info); ;
    t->startTime.tv_usec = 0;

    char buffer[80];
    printf("%10d | %9ld | %9ld | Create Thread \n", t->id, nowtime.tv_sec, nowtime.tv_usec);
//    fprintf(fp, "%10d | %9ld | %9ld | Create Thread \n", t->id, nowtime.tv_sec, nowtime.tv_usec);

    strftime(buffer, sizeof(buffer)+2*sizeof(char), "%c\n", &time_info );
    printf("Set Timer %d run at ",t->id);
    printf(buffer);
  //  fprintf(fp,"Set Timer %d run at ",t->id);
  //  fprintf(fp,buffer);


    pthread_t *thread = (pthread_t *)malloc(sizeof(pthread_t));
    pthread_create(thread, NULL, waitFucntion,(Timer *)t);

    return thread;
}

void timerInit(Timer *t,unsigned int Period,unsigned int TaskToExecute,unsigned int  StartDelay,
               void *(*StartFcn)(void *),void *(*StopFcn)(void *),void *(*TimerFcn)(void *),void *(*ErrorFcn)(void *),
               void *UserData)
{

    static int id = 0;
    id++;

    t->Period=Period;
    t->TaskToExecute=TaskToExecute;
    t->StartDelay=StartDelay;

    t->UserData=UserData;
    t->StartFcn=StartFcn;
    t->StopFcn=StopFcn;
    t->TimerFcn=TimerFcn;
    t->ErrorFcn=ErrorFcn;
    t->id=id;
}
