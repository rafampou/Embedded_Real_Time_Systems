#include "queue.h"

queue *timerQueue;  // The main global queue for timers
extern  pthread_mutex_t *time_mux;

void *consumer(void* arg)
{

  while (1) {   // main loop for each timer consumer. Wait for tasks to execute

    pthread_mutex_lock (timerQueue->mut);
    while (timerQueue->empty) {
      ////printf ("\nconsumer: queue EMPTY.\n");
      pthread_cond_wait (timerQueue->notEmpty, timerQueue->mut);
    }

    struct timeval before,after;
    gettimeofday(&before,NULL);

    job *t = queueDel();



    pthread_mutex_unlock (timerQueue->mut);
    pthread_cond_signal (timerQueue->notFull);

    gettimeofday(&after,NULL);
  //  fprintf(fp, "Cosumer time %f usec \n", (after.tv_sec - before.tv_sec)*10e6 +(after.tv_usec - before.tv_usec) );

    (t->jobFcn)(t->data);

    free(t);
    //usleep (50000);
  }
  return (NULL);
  pthread_exit(0);
}


queue *queueInit (void)
{
  timerQueue = (queue *)malloc (sizeof (queue));
  if (timerQueue == NULL) return (NULL);

  timerQueue->empty = 1;
  timerQueue->full = 0;
  timerQueue->head = 0;
  timerQueue->tail = 0;
  timerQueue->mut = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
  pthread_mutex_init (timerQueue->mut, NULL);
  timerQueue->notFull = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (timerQueue->notFull, NULL);
  timerQueue->notEmpty = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (timerQueue->notEmpty, NULL);

  return (timerQueue);
}

void queueDelete (void)
{
  pthread_mutex_destroy (timerQueue->mut);

  free (timerQueue->mut);
  pthread_cond_destroy (timerQueue->notFull);
  free (timerQueue->notFull);
  pthread_cond_destroy (timerQueue->notEmpty);
  free (timerQueue->notEmpty);
  free (timerQueue);
}

void queueAdd (job *in)
{
  timerQueue->buf[timerQueue->tail] = in;

  timerQueue->tail++;
  if (timerQueue->tail == QUEUESIZE)
    timerQueue->tail = 0;
  if (timerQueue->tail == timerQueue->head)
    timerQueue->full = 1;
  timerQueue->empty = 0;

  return;
}

job *queueDel (void)
{

  job *out = (job *)timerQueue->buf[timerQueue->head];

  timerQueue->head++;
  if (timerQueue->head == QUEUESIZE)
    timerQueue->head = 0;
  if (timerQueue->head == timerQueue->tail)
    timerQueue->empty = 1;
  timerQueue->full = 0;

  return out;
}
