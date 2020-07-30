// Compile with gcc prod-cons.c -o prod-cons -pthread -lm


#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>



#define QUEUESIZE 10


void *producer (void *args);
void *consumer (void *args);

double avgdrift;
float totaltime;
float driftsum;
struct workFunction
{
    void * (*work)(void *);
    void * arg;
    struct timeval time_added;
    

};


typedef struct
{
    struct workFunction buf[QUEUESIZE];
    long head, tail;
    int full, empty;
    pthread_mutex_t *mut;
    pthread_cond_t *notFull, *notEmpty;
} queue;

typedef struct {

    long Period;  // in us.
    int TasksToExecute; // > 0 , iterations.
    int StartDelay;
    void * (*StartFcn)(void *);
    void * (*StopFcn)(void *);
    void * (*TimerFcn)(void *);
    void * (*ErrorFcn)(void *);
    void * UserData;

    queue *q;
    pthread_t thread_id;
} Timer ;

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

void start(Timer *t);

int producer_count;
int consumer_count = 10;



int main (int argc, char *argv[])
{
    

    pthread_t producer_threads[10];
    pthread_t consumer_threads[consumer_count];

    queue *fifo;
    fifo = queueInit ();

    if (fifo ==  NULL)
    {
        fprintf (stderr, "Queue Init failed.\n");
        exit (1);
    }


    // Thread creation

    Timer *timer_1 = malloc(sizeof(Timer));
    int taskcount = 10000;
    timer_1->TimerFcn = tasks[1];
    timer_1->TasksToExecute = taskcount;
    timer_1->UserData = 3;
    timer_1->q = fifo;
    timer_1->Period = 10000;
    start(timer_1);
    producer_threads[0] = timer_1->thread_id;

    for(int i = 0 ; i < consumer_count ; i ++){
    pthread_create(&consumer_threads[i], NULL, consumer, fifo);
    }

    pthread_join(timer_1->thread_id, NULL);
    
    printf("Mean drift %f us!\n",avgdrift);

   //printf("Mean drift %f us!\n",driftsum / (float)taskcount);
    
    for(int i = 0 ; i < consumer_count ; i ++){
    pthread_join(consumer_threads[i], NULL);
    }
    
    queueDelete (fifo);
    free(timer_1);
    
    return 0;
}

void start(Timer *t) {
    pthread_create(&(t->thread_id), NULL, producer, t);

}

void *producer (void *t)
{
    Timer *timer = (Timer *)t;
    queue *fifo;
    fifo = (queue *)(timer->q);
    
    struct workFunction task;
    task.arg = timer->UserData; 
    task.work = timer->TimerFcn;
    struct timeval timestamp;
    long long lastCallback = 0;
    long long timepassed;
    int timedrift;
    long long waittime;
    driftsum = 0;

    for (int i = 0; i < timer->TasksToExecute; i++)
    {

        pthread_mutex_lock (fifo->mut);

        while (fifo->full){
            printf ("\nproducer: queue FULL.\n");
            //timer->ErrorFcn();
            pthread_cond_wait (fifo->notFull, fifo->mut);
        }


        
        

        
        queueAdd (fifo, task);
        gettimeofday(&timestamp, NULL);
        long long s = (long long)((long long)timestamp.tv_sec * 1000000 + (long long)timestamp.tv_usec);
        
        
        pthread_mutex_unlock (fifo->mut);
        pthread_cond_signal (fifo->notEmpty);
        
        timepassed = s - lastCallback;
        //timedrift = timepassed - waittime; // if other way.
        timedrift = timepassed - timer->Period; // was timepassed - waitime
        printf("\nproducer: I added an item ( id %d ) at %f\n",count+1,s/1000000.0);
        printf("lasttime : %lld\n",lastCallback);
        printf("s : %lld\n",s);
        printf("Time that has passed in ms : %f\n",timepassed / 1000.0);
        printf("Time drift in ms = %f\n",timedrift / 1000.0);
        
        if(i == 0){
            waittime = timer->Period;}
        else{
            avgdrift = (avgdrift * (i-1) + timedrift) / i; // mean drift way
            waittime = timer->Period  -  avgdrift;  // was period - timedrift
            totaltime += timepassed / 1000.0;
            driftsum += timedrift;
            printf("Should have been %f \n" ,(timer->Period / 1000.0 ) * i);
            printf("But it is %f\n",totaltime);
            printf("Average drift %f \n",avgdrift);}
        
        printf("Sleeping for %lld us \n",waittime);
        lastCallback = s;
        if(waittime < 0) {waittime = 0;}
        
        
        
        usleep(waittime);
}    
    return (NULL);
}

void *consumer (void *q)
{
    queue *fifo;
    struct workFunction  task;


    fifo = (queue *)q;

    long long dt;
    struct timeval t_start, t_end;
    double milliseconds;

    while(1)
    {

        pthread_mutex_lock (fifo->mut);
        while (fifo->empty)
        {
            printf ("\nconsumer: queue EMPTY.\n");
            pthread_cond_wait (fifo->notEmpty, fifo->mut);


        }

        task = fifo->buf[fifo->head];

        gettimeofday(&t_end, NULL);


        
        

        queueDel (fifo, &task);

        t_start = task.time_added;
        long long s = (long long)((long long)t_start.tv_sec * 1000000 + (long long)t_start.tv_usec);
        long long e = (long long)((long long)t_end.tv_sec * 1000000 + (long long)t_end.tv_usec);
        long long dt =  (long long)(e-s);
        average = (double)(average * count + dt)/(double)(++count);
        printf("\nconsumer: I deleted item no %d in %lld microseconds and average time is %f\n",count,dt,average);
        

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
