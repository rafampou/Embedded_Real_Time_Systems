#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h> //gettimeofday()


#define consumers 3

#define RUN_SECONDS 10

#include "queue.h"
#include "timer.h"
extern queue *timerQueue;  // The main global queue for timers


FILE *fp;   // file print


int startfn(Timer *t){
    struct timeval nowtime;
    gettimeofday(&nowtime,NULL);
    //fprintf(fp, "%10d | %9ld | %9ld | startfn \n", t->id, nowtime.tv_sec, nowtime.tv_usec);
    printf("%10d | %9ld | %9ld | startfn \n", t->id, nowtime.tv_sec, nowtime.tv_usec);
    return 1;
}
int stopfn(Timer *t){

    struct timeval nowtime;
    gettimeofday(&nowtime,NULL);
    //fprintf(fp, "%10d | %9ld | %9ld | stopfn \n", t->id, nowtime.tv_sec, nowtime.tv_usec);
    printf("%10d | %9ld | %9ld | stopfn \n", t->id, nowtime.tv_sec, nowtime.tv_usec);
    return 2;
}
int timefn(int *t){

    struct timeval nowtime;
    gettimeofday(&nowtime,NULL);
    fprintf(fp, "%3d | timefnc timestamp %10d sec %10d sec\n", t[2], nowtime.tv_sec, nowtime.tv_usec);
    //printf("%10d | %9ld | %9ld | timefn \n", t->id, nowtime.tv_sec, nowtime.tv_usec);
    int d = t[0] > 0 ? t[0] : 1000;
    int n = t[1] > 0 ? t[1] : 1000;

    double *randomArray = (double *)malloc(n*sizeof(double));

    for (int i = 0; i < n-1; i++) {
      struct timeval randtime;
      gettimeofday(&randtime,NULL);
      randomArray[0]=1;
      for (int j = 0; j < d; j++) {
        double randomNum = randtime.tv_usec*(j+i+1);
        gettimeofday(&randtime,NULL);
        randomArray[i]= randomArray[i] + randomNum+randtime.tv_usec;
        randomArray[i+1] = randtime.tv_usec * randtime.tv_sec;
      }

    }
    free(randomArray);

    return 0;
}
int errorfns(Timer *t){

    struct timeval nowtime;
    gettimeofday(&nowtime,NULL);
    //fprintf(fp, "%10d | %9ld | %9ld | errorfns \n", t->id, nowtime.tv_sec, nowtime.tv_usec);
    printf("%10d | %9ld | %9ld | errorfns \n", t->id, nowtime.tv_sec, nowtime.tv_usec);

    return 4;
}


int main(int argc, char const *argv[]){

    int numberOfTimers = 3;
    int sizesOfN[3];
    sizesOfN[0]=0;
    sizesOfN[1]=0;
    sizesOfN[2]=0;

    if(argc < 5 && argc > 1)
    {
      for (int i = 1; i < argc; i++)
        sizesOfN[i-1] = atoi(argv[i]);

    }else{
        sizesOfN[0]=500;
        sizesOfN[1]=500;
        sizesOfN[2]=500;
    }

    queue *mainQueue = queueInit();


    fp = fopen("Timers_Timestamps.txt", "w+");


    pthread_t *cons = (pthread_t *)malloc(consumers*sizeof(pthread_t));

    for (int i = 0; i < consumers; i++)
      pthread_create (&cons[i], NULL, consumer, mainQueue);


    int newdata1[3]= {sizesOfN[0],300,1};
    Timer *newTimer1 = (Timer *)malloc(sizeof(Timer));
    timerInit(newTimer1,1000,RUN_SECONDS,2,(void * (*)(void *))&startfn,(void * (*)(void *))&stopfn,(void * (*)(void *))&timefn,(void * (*)(void *))&errorfns,newdata1);


    int newdata2[3]= {sizesOfN[1],300,2};
    Timer *newTimer2 = (Timer *)malloc(sizeof(Timer));
    timerInit(newTimer2,100,RUN_SECONDS*10,2,(void * (*)(void *))&startfn,(void * (*)(void *))&stopfn,(void * (*)(void *))&timefn,(void * (*)(void *))&errorfns,newdata2);


    int newdata3[3]= {sizesOfN[2],300,3};
    Timer *newTimer3 = (Timer *)malloc(sizeof(Timer));
    timerInit(newTimer3,10,RUN_SECONDS*100,2,(void * (*)(void *))&startfn,(void * (*)(void *))&stopfn,(void * (*)(void *))&timefn,(void * (*)(void *))&errorfns,newdata3);


    struct timeval timenow;
    gettimeofday(&timenow,NULL);
    struct tm *info = localtime((const time_t*)&timenow);
    // printf("%d %d %d %d %d %d",info->tm_year+1900,info->tm_mon + 1,info->tm_mday,info->tm_hour +0,info->tm_min +1,info->tm_sec +0);

    pthread_t *thread1 = NULL;
    if(sizesOfN[0]>0 ){
        printf("Set Timer 1 with n= %d \n",sizesOfN[0] );
        thread1 = startat(newTimer1,info->tm_year+1900,info->tm_mon +1 ,info->tm_mday,info->tm_hour -1 ,info->tm_min +0,info->tm_sec +5);
    }

    pthread_t * thread2 = NULL;
    if(sizesOfN[1]>0 ){
        printf("Set Timer 2 with n= %d \n",sizesOfN[1] );
        thread2 = startat(newTimer2,info->tm_year+1900,info->tm_mon +1 ,info->tm_mday,info->tm_hour -1 ,info->tm_min +0,info->tm_sec +5);
    }

    pthread_t * thread3 = NULL;
    if(sizesOfN[2]>0 ){
        printf("Set Timer 3 with n= %d \n",sizesOfN[2] );
        thread3 = startat(newTimer3,info->tm_year+1900,info->tm_mon +1 ,info->tm_mday,info->tm_hour -1 ,info->tm_min +0,info->tm_sec +5);
    }
    if(sizesOfN[0]>0 )
      pthread_join(*thread1,NULL);
    if(sizesOfN[1]>0 )
      pthread_join(*thread2,NULL);
    if(sizesOfN[2]>0 )
        pthread_join(*thread3,NULL);


    while (!(timerQueue->empty)) ;

    pthread_mutex_lock (timerQueue->mut);
    for (int i = 0; i < consumers; i++)
        pthread_cancel (cons[i]);
    pthread_mutex_unlock (timerQueue->mut);

    queueDelete();
    free(newTimer1);
    free(newTimer2);
    free(newTimer3);

    fclose(fp);
    usleep(1000);

    return 0;
}
