#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS  2

void PrintHello()
{
   
   printf("Hello\n");
   pthread_exit(NULL);
}

void PrintWorld()
{
   
   printf("World\n");
   pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
    pthread_t thread1,thread2;  
    pthread_attr_t atrr1,atrr2;

    int priority_high=99, priority_low=1;
    int status=0,s=0;
    int t = 10;

    struct sched_param para1;
    struct sched_param para2;  

    para1.sched_priority = sched_get_priority_max(SCHED_FIFO);
    para2.sched_priority = sched_get_priority_min(SCHED_FIFO);

    status = pthread_attr_init(&atrr1);
    if(status!=0)
    {
        printf("\nAttribute init failed !\n");
        return 0;
    }

    status = pthread_attr_init(&atrr2);
    if(status!=0)
    {
                printf("\nAttribute init failed !\n");
                return 0;
    }   

    status = pthread_attr_setschedpolicy(&atrr1, SCHED_FIFO);
    if(status!=0)
        {
                printf("\nSched policy init failed 1!\n");
                return 0;
        }
    status = pthread_attr_setschedpolicy(&atrr2, SCHED_FIFO);
        if(status!=0)
        {
                printf("\nSched policy init failed 2!\n");
                return 0;
        }
 
    status = pthread_attr_setschedparam(&atrr1, &para1);
    if(s!=0)
        {
                printf("\nCould not set Priority : 1 !\n");
                return 0;
        }
  // pthread_attr_setinheritsched (&atrr1, PTHREAD_INHERIT_SCHED);

    status = pthread_attr_setschedparam(&atrr2, &para2);
        if(s!=0)
        {
                printf("\nCould not set priority : 2!\n");
                return 0;
        } 
    //pthread_attr_setinheritsched (&atrr2, PTHREAD_EXPLICIT_SCHED);

    int rc1,rc2;
      
      rc1 = pthread_create(&thread1, &atrr1, (void *)&PrintHello, NULL);
      
      if (rc1)
      {
         printf("ERROR; return code from pthread_create() is %d\n", rc1);
         
      }
      //printf("Main It's me, thread #%lu\n", thread1);

      rc2 = pthread_create(&thread2, &atrr2, (void *)&PrintWorld, NULL);
      
      if (rc2)
      {
      printf("ERROR; return code from pthread_create() is %d\n", rc2);
      }
      
      pthread_exit(NULL);
}
