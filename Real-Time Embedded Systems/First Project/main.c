#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>


#define QUEUESIZE 10
#define LOOP 20

void *producer (void *args);
void *consumer (void *args);



struct workFunction
{
    void * (*work)(void *);
    void * arg;
    struct timeval time_added;

};

typedef struct {
  struct workFunction buf[QUEUESIZE];
  long head, tail;
  int full, empty;
  pthread_mutex_t *mut;
  pthread_cond_t *notFull, *notEmpty;
} queue;


int count = 0;
unsigned long average = 0;

typedef void * (*work)(void *); // For readability

void * lotr(void *p) { printf("Fly you fools!\n"); }
void * matrix(void *p) { printf("Blue pill or red pill? It's your choise.\n");}
void * harrypotter(void *p) { printf("You're a wizard Harry!\n"); }
void * snitch(void *p) {printf("Snitches get stiches.\n");}

work tasks[4] = {lotr,matrix,harrypotter,snitch};



queue *queueInit (void);
void queueDelete (queue *q);
void queueAdd (queue *q, struct workFunction in);
void queueDel (queue *q, struct workFunction *out);

int main ()
{
  queue *fifo;

  // Number of producer and consumer threads
  int producer_count = 50;
  int consumer_count = 100;

  pthread_t producer_threads[producer_count];
  pthread_t consumer_threads[consumer_count];

  fifo = queueInit ();

  if (fifo ==  NULL) {
    fprintf (stderr, "main: Queue Init failed.\n");
    exit (1);
  }

  // Thread creation

  for(int i = 0; i < producer_count; i++)
    pthread_create(&producer_threads[i], NULL, producer, fifo);

  for(int j = 0; j < consumer_count; j++)
    pthread_create(&consumer_threads[j], NULL, consumer, fifo);

  // Thread joining

  for(int k = 0; k < producer_count; k++)
    pthread_join(producer_threads[k], NULL);
  for(int l = 0; l < consumer_count; l++)
    pthread_join(consumer_threads[l], NULL);



  queueDelete (fifo);

  return 0;
}

void *producer (void *q)
{
  queue *fifo;
  fifo = (queue *)q;

  struct workFunction task;

  struct timeval tv;
  //unsigned long time;


  for (int i = 0; i < LOOP; i++) {

    pthread_mutex_lock (fifo->mut);

    while (fifo->full)
        pthread_cond_wait (fifo->notFull, fifo->mut);


    task.arg = pthread_self(); // Change this
    task.work = tasks[pthread_self() % 4];

    gettimeofday(&tv, NULL);
    task.time_added = tv;

    queueAdd (fifo, task);



    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notEmpty);
   // printf("producer: added %d \n",i);


  }

  return (NULL);
}

void *consumer (void *q)
{
  queue *fifo;
  struct workFunction  task;

  fifo = (queue *)q;

  unsigned long dt;
  struct timeval t_start, t_end;

  unsigned long end, start;

  //FILE *fptr;


  while(1) {
    pthread_mutex_lock (fifo->mut);
    while (fifo->empty) {

      pthread_cond_wait (fifo->notEmpty, fifo->mut);
    }

    task = fifo->buf[fifo->head];

    gettimeofday(&t_end, NULL);


    task.work(task.arg);

    queueDel (fifo, &task);



    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notFull);

    t_start = task.time_added;
    dt  =  (t_end.tv_sec  - t_start.tv_sec)*1000000 + t_end.tv_usec-t_start.tv_usec;
    average = (average * count + dt)/(++count);
   /* fptr = fopen("delay.txt","a");
    fprintf(fptr,"%lu\n",dt);
    fclose(fptr);
*/
    end = (t_end.tv_sec * 1000000)  + t_end.tv_usec;
        start = (t_start.tv_sec * 1000000)  + t_start.tv_usec;
        printf ("consumer: I deleted an item added at time %lu in time %lu and dt = %lu .\n", start , end, dt);
        printf("%d    %lu\n",count,average);


  }

  return (NULL);
}



queue *queueInit (void)
{
  queue *q;

  q = (queue *)malloc (sizeof (queue));
  if (q == NULL) return (NULL);

  q->empty = 1;
  q->full = 0;
  q->head = 0;
  q->tail = 0;
  q->mut = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
  pthread_mutex_init (q->mut, NULL);
  q->notFull = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (q->notFull, NULL);
  q->notEmpty = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (q->notEmpty, NULL);

  return (q);
}

void queueDelete (queue *q)
{
  pthread_mutex_destroy (q->mut);
  free (q->mut);
  pthread_cond_destroy (q->notFull);
  free (q->notFull);
  pthread_cond_destroy (q->notEmpty);
  free (q->notEmpty);
  free (q);
}

void queueAdd (queue *q, struct workFunction in)
{
  q->buf[q->tail] = in;
  q->tail++;
  if (q->tail == QUEUESIZE)
    q->tail = 0;
  if (q->tail == q->head)
    q->full = 1;
  q->empty = 0;

  return;
}

void queueDel (queue *q, struct workFunction *out)
{
  *out = q->buf[q->head];

  q->head++;
  if (q->head == QUEUESIZE)
    q->head = 0;
  if (q->head == q->tail)
    q->empty = 1;
  q->full = 0;

  return;
}
