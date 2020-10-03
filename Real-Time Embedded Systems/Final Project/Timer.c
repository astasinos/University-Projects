// Compile with gcc prod-cons.c -o prod-cons -pthread -lm


#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>



#define QUEUESIZE 5




void *producer (void *args);
void *consumer (void *args);




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

typedef struct {                // Timer struct.

    int Period;  // in ms.
    int TasksToExecute; // > 0 , iterations.
    unsigned long long StartDelay;      // secs
    void * (*StartFcn)();
    void * (*StopFcn)(void *);
    void * (*TimerFcn)(void *);
    void * (*ErrorFcn)();
    void * UserData;
    queue *q;
    pthread_t thread_id;

    int beginImmediately;               // if timer is supposed to start immediately
    unsigned long long timeToStart;     // if not, how many usecs to wait until start.
    

   

} Timer ;



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
void timerInit(Timer *t, int Period, int TasksToExecute, unsigned long long StartDelay, void * (*StartFcn)(), void * (*StopFcn)(void *), void * (*TimerFcn)(void *), void * (*ErrorFcn)(), void * UserData, queue *q);
void * startTimerFcn();
void * stopTimerFcn(void * t);
void * ErrorTimerFcn();

int producer_count = 3;                 // 3 Timers.
int consumer_count  = 10;               

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
    // Initialize timers
    timerInit(timer_1,10,360001,0,startTimerFcn,stopTimerFcn,tasks[1],ErrorTimerFcn,4,fifo);
    timerInit(timer_2,100,36001,0,startTimerFcn,stopTimerFcn,tasks[2],ErrorTimerFcn,4,fifo);
    timerInit(timer_3,1000,3601,0,startTimerFcn,stopTimerFcn,tasks[3],ErrorTimerFcn,4,fifo);
    
    //startat(timer_1,2020-1900,9,02,17,50,0);
    // Start timers
    start(timer_1);
    start(timer_2);
    start(timer_3);
    producer_threads[0] = timer_1->thread_id;
    producer_threads[1] = timer_2->thread_id;
    producer_threads[2] = timer_3->thread_id;

    for(int i = 0 ; i < consumer_count ; i ++){
    pthread_create(&consumer_threads[i], NULL, consumer, fifo);
    }
   // wait for timers to stop
   pthread_join(timer_1->thread_id, NULL);
   pthread_join(timer_2->thread_id, NULL);
   pthread_join(timer_3->thread_id, NULL);

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
    t->beginImmediately = 1;                            // start timer immediately
    pthread_create(&(t->thread_id), NULL, producer, t);

}

// Start timer at specific moment with seconds accuracy.

void startat(Timer * t, int year, int month, int day, int hour, int min, int sec){
    
  struct tm timeTostartExec;
  time_t daytime;
  unsigned long long timenow_usecs,delaytime;
  struct timeval timenow;             // what the time now is.
  gettimeofday(&timenow, NULL);
  timenow_usecs = (unsigned long long)timenow.tv_sec*1000000 + (unsigned long long)timenow.tv_usec;//time in usecs
  
  timeTostartExec.tm_year = year; 
  timeTostartExec.tm_mon = month;          
  timeTostartExec.tm_mday = day;          
  timeTostartExec.tm_hour = hour;
  timeTostartExec.tm_min = min;
  timeTostartExec.tm_sec = sec;
  timeTostartExec.tm_isdst = -1;        // no dst.

  daytime = mktime(&timeTostartExec);
  delaytime = (daytime*1000000) - timenow_usecs;        // usecs until start.
  
  t->timeToStart = delaytime;
  t->beginImmediately = 0;
  pthread_create (&(t->thread_id), NULL, producer, t);
}

// Helper function for timer initialization.

void timerInit(Timer *t, int Period, int TasksToExecute, unsigned long long StartDelay, void * (*StartFcn)(), 
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
   
    printf("I have completed my purpose...I must now stop!\n");

}
void * ErrorTimerFcn(){

    printf("Uhmm, maybe the queue is full..I must now wait.\n");
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
    long long timepassed, s;
    int timedrift;
    
    long long waittime = timer->Period * 1000;  // convert to us.
    
    if(timer->beginImmediately == 0) {          // if started with startat wait the appropriate time.

        
        printf("I'm going to sleep for %lld us \n", timer->timeToStart);
        usleep(timer->timeToStart);

    }

    timer->StartFcn();
    
    usleep(timer->StartDelay * 1000000); // Start delay is given in seconds.

    for (int i = 0; i < timer->TasksToExecute; i++)
    {
        
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
       
         if(i > 0){
            timepassed = s - lastCallback;                      // how much time has passed since the last execution
            timedrift = timepassed - (timer->Period * 1000);      // how much this time differs from our period
            waittime = waittime -   timedrift;                  
            }
        
        lastCallback = s;
        
        if(waittime < 0) {waittime = 0;}                    // if drift is so big that waitime becomes negative then set it to zero.

        usleep(waittime);
}   
    
    timer->StopFcn(timer);
    
    return (NULL);
}



void *consumer (void *q)
{
    queue *fifo;
    struct workFunction  task;


    fifo = (queue *)q;

    
    while(1)
    {
        
        
        pthread_mutex_lock (fifo->mut);
        while (fifo->empty)
        {
            printf ("\nconsumer: queue EMPTY.\n");
            pthread_cond_wait (fifo->notEmpty, fifo->mut);


        }

        task = fifo->buf[fifo->head];

        
        queueDel (fifo, &task);
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
