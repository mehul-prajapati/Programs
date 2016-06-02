#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS  2



void *PrintHello(void *threadid)   // Subroutine for thread
{
   long tid;
   int y;
   
   pthread_t p;
   tid = (long)threadid;
   printf("Hello World! It's me, thread #%ld!\n", tid);
   p=pthread_self();
   printf("From PrintHello thread id: %lu\n",p);
   
   y= sched_get_priority_max(SCHED_RR); 
   printf("Pririty:y=%d\n",y);
   pthread_exit(NULL);
}

int main ()
{
   
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;   
	size_t stacksize;
	pthread_attr_t attr;      // Declaring a pthread attribute variable of the pthread_attr_t data type

	pthread_attr_init(&attr);    // Initializing the attribute variable with pthread_attr_init()
	pthread_attr_getstacksize (&attr, &stacksize);
	printf("Default stack size = %li\n",(long int) stacksize);

	for(t=1; t<=NUM_THREADS; t++)
	{

		printf("In main: creating thread %ld\n", t);
		rc = pthread_create(&threads[t], &attr, PrintHello, (void *)t)
		printf("From main thread id: %lu\n",threads[t]);
		
		if (rc)
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	/* Last thing that main() should do */

	pthread_exit(NULL);
}
