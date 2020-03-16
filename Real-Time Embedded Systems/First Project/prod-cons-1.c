#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define QUEUESIZE 10
#define LOOP 5
#define NUM_THREADS 2

void *producer (void *args);
void *consumer (void *args);

void test(void *p)
{
    int arg = (int)p;
    printf("\nIt works! This was a function call for item id #%d \n", arg);
}

struct workFunction
{
    void * (*work)(void *);
    void * arg;
    int id;
};



typedef struct
{
    struct workFunction buf[QUEUESIZE];
    long head, tail;
    int full, empty;
    pthread_mutex_t *mut;
    pthread_cond_t *notFull, *notEmpty;
    sem_t *sem;





} queue;

queue *queueInit (void);
void queueDelete (queue *q);
void queueAdd (queue *q, struct workFunction in);
void queueDel (queue *q, struct workFunction *out);

int main ()
{


    queue *fifo;
    pthread_t prod_threads[NUM_THREADS];
    pthread_t con_threads[NUM_THREADS];

    fifo = queueInit ();
    if (fifo ==  NULL)
    {
        fprintf (stderr, "main: Queue Init failed.\n");
        exit (1);
    }
    for(int i = 0 ; i < NUM_THREADS; i++)
    {

        pthread_create (&prod_threads[i], NULL, producer, fifo);
        pthread_create (&con_threads[i], NULL, consumer, fifo);

    }

    for(int t = 0; t < NUM_THREADS; t++)
    {
        pthread_join(prod_threads[t], NULL);
        pthread_join(con_threads[t], NULL);

    }

    queueDelete (fifo);

    return 0;
}

void *producer (void *q)
{
    queue *fifo;
    struct workFunction func;

    fifo = (queue *)q;
    int i;
    for (i = 0; i < LOOP; i++)
    {

        int id;
        pthread_mutex_lock (fifo->mut);


        func.work = test;
        id = fifo->tail;
        func.arg = id;
        func.id = id;

        queueAdd (fifo, func);
        pthread_cond_signal(fifo->notEmpty);

        pthread_mutex_unlock (fifo->mut);
        sem_post(fifo->sem);
        printf ("\nproducer: added item with id #%d.\n", id );
    }

    return (NULL);
}

void *consumer (void *q)
{
    queue *fifo;
    struct workFunction d;

    fifo = (queue *)q;


    while(1)
    {
        sem_wait (fifo->sem);
        pthread_mutex_lock (fifo->mut);
        while(fifo->empty)
        {

            pthread_cond_wait(fifo->notEmpty, fifo->mut);



        }
        d = fifo->buf[fifo->head];
        d.work(d.arg);
        int id = d.id;
        queueDel (fifo, &d);

        pthread_mutex_unlock (fifo->mut);
        printf ("\nconsumer: deleted item with id #%d.\n", id);
    }

    return (NULL);
}

#if 0
typedef struct
{
    int buf[QUEUESIZE];
    long head, tail;
    int full, empty;
    pthread_mutex_t *mut;
    pthread_cond_t *notFull, *notEmpty;
} queue;
#endif

queue *queueInit ()
{
    queue *q;

    q = (queue *)malloc (sizeof (queue));
    if (q == NULL)
        return (NULL);




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
    q->sem = (sem_t *) malloc (sizeof (sem_t));
    sem_init(q->sem, 0, 0);

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
    sem_destroy (q->sem);
    free (q->sem);
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

