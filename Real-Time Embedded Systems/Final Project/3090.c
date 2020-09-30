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

    int Period;  // in ms.
    int TasksToExecute; // > 0 , iterations.
    int StartDelay;
    void * (*StartFcn)();
    void * (*StopFcn)(void *);
    void * (*TimerFcn)(void *);
    void * (*ErrorFcn)();
    void * UserData;

    queue *q;
    pthread_t thread_id;

    // Stats.
    double absavgdrift;
    double totaltime;
    int maxdrift;
    long long max_producer_add_time;
    long long  min_producer_add_time;
    double avg_producer_add_time;
    
   

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
void startat(Timer *t,int year, int month, int day, int hour, int minute, int second);
void timerInit(Timer *t, int Period, int TasksToExecute, int StartDelay, void * (*StartFcn)(), void * (*StopFcn)(void *), void * (*TimerFcn)(void *), void * (*ErrorFcn)(), void * UserData, queue *q);
void * startTimerFcn();
void * stopTimerFcn(void * t);
void * ErrorTimerFcn();

int producer_count = 3;
int consumer_count = 10;
FILE *consfile = NULL;
int main (int argc, char *argv[])
{
    

    pthread_t producer_threads[producer_count];
    pthread_t consumer_threads[consumer_count];

    queue *fifo;
    fifo = queueInit ();

    if (fifo ==  NULL)
    {
        fprintf (stderr, "Queue Init failed.\n");
        exit (1);
    }

    Timer *timer_1 = malloc(sizeof(Timer));
    Timer *timer_2 = malloc(sizeof(Timer));
    Timer *timer_3 = malloc(sizeof(Timer));
    timerInit(timer_1,10,360001,0,startTimerFcn,stopTimerFcn,tasks[1],ErrorTimerFcn,4,fifo);
    timerInit(timer_2,100,36001,0,startTimerFcn,stopTimerFcn,tasks[1],ErrorTimerFcn,4,fifo);
    timerInit(timer_3,1000,3601,0,startTimerFcn,stopTimerFcn,tasks[1],ErrorTimerFcn,4,fifo);
    start(timer_1);
    //start(timer_2);
    //start(timer_3);
    producer_threads[0] = timer_1->thread_id;
    //producer_threads[1] = timer_2->thread_id;
    //producer_threads[2] = timer_3->thread_id;

    
    consfile = fopen("line_waittime.txt","w");

    for(int i = 0 ; i < consumer_count ; i ++){
    pthread_create(&consumer_threads[i], NULL, consumer, fifo);
    }

    pthread_join(timer_1->thread_id, NULL);
   //pthread_join(timer_2->thread_id, NULL);
   //pthread_join(timer_3->thread_id, NULL);
/*
    printf("10 ms Period\nDrift --> Mean: %f us  Max: %d us\n",timer_1->absavgdrift,timer_1->maxdrift);
    printf("Producer Add time --> Mean: %f us  Max: %lld us  Min: %lld us\n",timer_1->avg_producer_add_time,timer_1->max_producer_add_time,timer_1->min_producer_add_time);
    printf("Took a total of %f ms\n",timer_1->totaltime);
  
    printf("100 ms Period\nDrift --> Mean: %f us  Max: %d us\n",timer_2->absavgdrift,timer_2->maxdrift);
    printf("Producer Add time --> Mean: %f us  Max: %lld us  Min: %lld us\n",timer_2->avg_producer_add_time,timer_2->max_producer_add_time,timer_2->min_producer_add_time);
    printf("Took a total of %f ms\n",timer_2->totaltime);

    printf("1 sec Period\nDrift --> Mean: %f us  Max: %d us\n",timer_3->absavgdrift,timer_3->maxdrift);
    printf("Producer Add time --> Mean: %f us  Max: %lld us  Min: %lld us\n",timer_3->avg_producer_add_time,timer_3->max_producer_add_time,timer_3->min_producer_add_time);
    printf("Took a total of %f ms\n",timer_3->totaltime);
*/
    for(int i = 0 ; i < consumer_count ; i ++){
    pthread_join(consumer_threads[i], NULL);
    }
    
    queueDelete (fifo);
    free(timer_1);
    free(timer_2);
    free(timer_3);
    return 0;
}

void start(Timer *t) {
    pthread_create(&(t->thread_id), NULL, producer, t);

}

void timerInit(Timer *t, int Period, int TasksToExecute, int StartDelay, void * (*StartFcn)(), 
                void * (*StopFcn)(void *), void * (*TimerFcn)(void *), void * (*ErrorFcn)(), void * UserData, queue *q) {
    
    t->Period = Period;
    t->TasksToExecute = TasksToExecute;
    t->StartDelay = StartDelay;
    t->StartFcn = StartFcn;
    t->StopFcn = StopFcn;
    t->TimerFcn = TimerFcn;
    t->ErrorFcn = ErrorFcn;
    t->UserData = UserData;
    t->q = q;
}

void * startTimerFcn(){

    printf("Just starting out!\n");
}
void * stopTimerFcn(void * t){
    
    Timer *tmr = (Timer *)t;
   // free(tmr);
    printf("Dobby is freeee!\n");

}
void * ErrorTimerFcn(){

    printf("Hey yo, somethings wrong man\n");
}
void *producer (void *t)
{
    Timer *timer = (Timer *)t;
    queue *fifo;
    fifo = (queue *)(timer->q);
    
    struct workFunction task;
    task.arg = timer->UserData; 
    task.work = timer->TimerFcn;

    struct timeval timestamp, beforeMutex;
    long long lastCallback = 0;
    long long timepassed, s, pre_mutex_capture;
    long long  producer_add_time = 0;
    long long  max_producer_add_time = 0;
    long long  min_producer_add_time = 0;
    double  avg_producer_add_time = 0;
    int timedrift;
    int absdrift = 0;

    int maxdrift = 0;  
    double absavgdrift = 0;
    double totaltime = 0;
    long long waittime = timer->Period * 1000;  // convert to us.
    FILE *ptr;
    char filenameperiod[20];
    sprintf(filenameperiod,"drifts_%d.txt",timer->Period);
    ptr = fopen(filenameperiod,"w");
    FILE *ptr_addtime;
    char filename_prod_add_time[30];
    sprintf(filename_prod_add_time,"addtime_%d.txt",timer->Period);
    ptr_addtime = fopen(filename_prod_add_time,"w");
    
    timer->StartFcn();
    usleep(timer->StartDelay * 1000000); // Start delay is given in seconds.

    for (int i = 0; i < timer->TasksToExecute; i++)
    {
        //gettimeofday(&beforeMutex, NULL);
        //pre_mutex_capture = (long long)((long long)beforeMutex.tv_sec * 1000000 + (long long)beforeMutex.tv_usec);
        pthread_mutex_lock (fifo->mut);

        while (fifo->full){
            printf ("\nproducer: queue FULL.\n");
            timer->ErrorFcn();
            pthread_cond_wait (fifo->notFull, fifo->mut);
        }

        
        queueAdd (fifo, task);
        gettimeofday(&timestamp, NULL);
        s = (long long)((long long)timestamp.tv_sec * 1000000 + (long long)timestamp.tv_usec);
        
        
        pthread_mutex_unlock (fifo->mut);
        pthread_cond_signal (fifo->notEmpty);
        //fflush(ptr);
        //fflush(ptr_addtime);
        //producer_add_time = s - pre_mutex_capture;
        //avg_producer_add_time = (avg_producer_add_time * (i) + producer_add_time) / (i+1);
        //printf("\nproducer: I added a task with ( id %d ) at %f and it took me like %lld us\n",count+1,s/1000000.0,producer_add_time);
        if(i == 0) {
            min_producer_add_time = producer_add_time;
        }

        if(i > 0){
            timepassed = s - lastCallback;
            timedrift = timepassed - (timer->Period * 1000);
           // printf("Time drift in us = %d\n",timedrift);
            absdrift = (timedrift > 0) ? timedrift : -timedrift;
          //  absavgdrift = (absavgdrift * (i-1) + absdrift) / i ;
            waittime = waittime -   timedrift; 
            //totaltime += timepassed / 1000.0;
            if(absdrift > maxdrift) maxdrift = absdrift;

            }
        
        //if(producer_add_time > max_producer_add_time) max_producer_add_time = producer_add_time;
        //if(producer_add_time < min_producer_add_time) min_producer_add_time = producer_add_time;
        lastCallback = s;
        if(waittime < 0) {waittime = 0;}
        
        //fprintf(ptr,"%d\n",timedrift);
        
        //fprintf(ptr_addtime,"%lld\n",producer_add_time);
        
        //printf("Max prod time : %lld\n",max_producer_add_time);
        //printf("Min prod time : %lld\n",min_producer_add_time);
       // printf("Max drift : %d\n",maxdrift);

        usleep(waittime);
}   
    
    fclose(ptr);
    fclose(ptr_addtime);

    //timer->absavgdrift = absavgdrift;
    //timer->totaltime = totaltime;
    //timer->maxdrift = maxdrift;
    //timer->avg_producer_add_time = avg_producer_add_time;
    //timer->max_producer_add_time = max_producer_add_time;
    //timer->min_producer_add_time = min_producer_add_time;
    timer->StopFcn(timer);
    
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
    long long cons_del_time;
    
    while(1)
    {
        
        
        pthread_mutex_lock (fifo->mut);
        while (fifo->empty)
        {
         //   printf ("\nconsumer: queue EMPTY.\n");
            pthread_cond_wait (fifo->notEmpty, fifo->mut);


        }

        task = fifo->buf[fifo->head];

        
        queueDel (fifo, &task);
        //gettimeofday(&t_end, NULL);
        //t_start = task.time_added;
        //long long s = (long long)((long long)t_start.tv_sec * 1000000 + (long long)t_start.tv_usec);
        //long long e = (long long)((long long)t_end.tv_sec * 1000000 + (long long)t_end.tv_usec);
        //long long dt =  (long long)(e-s);
        
        //fprintf(consfile,"%lld\n",dt);
        //fflush(consfile);
        //average = (double)(average * count + dt)/(double)(++count);
        //printf("\nconsumer: I deleted item no %d in %lld microseconds and average time is %f\n",count,dt,average);
        

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
