#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>



#define QUEUESIZE 10
//#define LOOP 20000
int LOOP;
void *producer (void *args);
void *consumer (void *args);



struct workFunction
{
    void * (*work)(void *);
    void * arg;
    struct timeval time_added;
    int id;

};

typedef struct
{
    struct workFunction buf[QUEUESIZE];
    long head, tail;
    int full, empty;
    pthread_mutex_t *mut;
    pthread_cond_t *notFull, *notEmpty;
} queue;

int id = 1;
int count = 0;
double average = 0;

typedef void * (*work)(void *); // For readability

void * sincalc(void *p)     // Calculates sin ( pi / argument )
{

    int x =  (int) p  ;
    double pi = 3.14159265357897;
    if(x!= 0 ){
        double result = sin(pi / (double)x );
        printf("sin(PI/%d) = %lf\n",x,result);
    }
    else{
        printf("Can't divide by zero!\n");
    }

}

void * poweroftwo(void *p)  // Calculates argument^2
{

    int x = (int) p;
    double result = pow(x,2.0);

    printf("Wanna know %d to the power of 2 ? It's %f\n",x,result);

}
void * sum(void *p) // Calculates the sum of the first X integers
{
    int x = (int)p;
    long int total = 0;
    for(int i = 1; i <= x ; i++)
        total += i;

    printf("Wanna know the sum of the first %d integers ? It's %ld\n",x,total);


}
void * dumbfunc(void *p)    // This...this just prints..
{
    int x = (int) p;
    printf("I JUST PRINT.....HERE'S YOU'R NUMBER %d\n",x);
}

work tasks[4] = {sincalc,poweroftwo,sum,dumbfunc};



queue *queueInit (void);
void queueDelete (queue *q);
void queueAdd (queue *q, struct workFunction in);
void queueDel (queue *q, struct workFunction *out);


int producer_count;
int consumer_count;


int main (int argc, char *argv[])
{
    queue *fifo;

    // Number of producer and consumer threads

    producer_count =  atoi(argv[1]);
    consumer_count =  atoi(argv[2]);
    LOOP = atoi(argv[3]);

    pthread_t producer_threads[producer_count];
    pthread_t consumer_threads[consumer_count];

    fifo = queueInit ();

    if (fifo ==  NULL)
    {
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

    for(int m = 0 ; m < consumer_count ; m++)
        pthread_join(consumer_threads[m], NULL);

    queueDelete (fifo);

    return 0;
}

void *producer (void *q)
{
    queue *fifo;
    fifo = (queue *)q;

    struct workFunction task;
    struct timeval t;

    for (int i = 0; i < LOOP; i++)
    {

        pthread_mutex_lock (fifo->mut);

        while (fifo->full){
            pthread_cond_wait (fifo->notFull, fifo->mut);
        }


        task.arg = (unsigned int)pthread_self() % 20000; // Change this
        task.work = tasks[(unsigned int)pthread_self() % 4];
        task.id = id++;


        queueAdd (fifo, task);
        gettimeofday(&t, NULL);
        long long s = (long long)((long long)t.tv_sec * 1000000 + (long long)t.tv_usec);

        printf("\nproducer: I added the item with id %u\n",task.id);
        pthread_mutex_unlock (fifo->mut);
        pthread_cond_signal (fifo->notEmpty);

    }

    return (NULL);
}

void *consumer (void *q)
{
    queue *fifo;
    struct workFunction  task;

    FILE *fptr;


    fifo = (queue *)q;

    long long dt;
    struct timeval t_start, t_end;
    double milliseconds;

    while(1)
    {

        pthread_mutex_lock (fifo->mut);
        while (fifo->empty)
        {


            pthread_cond_wait (fifo->notEmpty, fifo->mut);


        }

        task = fifo->buf[fifo->head];

        gettimeofday(&t_end, NULL);


        
        int taskid = task.id;

        queueDel (fifo, &task);

        t_start = task.time_added;
        long long s = (long long)((long long)t_start.tv_sec * 1000000 + (long long)t_start.tv_usec);
        long long e = (long long)((long long)t_end.tv_sec * 1000000 + (long long)t_end.tv_usec);
        long long dt =  (long long)(e-s);
        average = (double)(average * count + dt)/(double)(++count);
        printf("\nconsumer: I deleted item id %d in %lld microseconds and average time is %f  %d\n",taskid,dt,average,consumer_count);
        if(count == producer_count * LOOP) {
            fptr = fopen("delay.txt","a");
            fprintf(fptr,"%f\n",average);
            fclose(fptr);

        }

        pthread_mutex_unlock (fifo->mut);
        pthread_cond_signal (fifo->notFull);
        task.work(task.arg);

        
        
    }

    return (NULL);
}



queue *queueInit (void)
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
    gettimeofday(&q->buf[q->tail].time_added, NULL);

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

