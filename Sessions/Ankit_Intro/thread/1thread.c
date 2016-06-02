#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS  2

int a=6;

void *PrintHello(void)
{
	printf("Hello \n");
	pthread_exit(NULL);
}

void *PrintWorld(void)
{
	printf("World\n");
	pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
    pthread_t thread1,thread2;  
    int rc1,rc2;
      
	rc1 = pthread_create(&thread1, NULL, PrintHello, NULL);

	if (rc1)
	{
		printf("ERROR; return code from pthread_create() is %d\n", rc1);
		return 0;
	}

	rc2 = pthread_create(&thread2, NULL, PrintWorld, NULL);

	if (rc2)
	{
		printf("ERROR; return code from pthread_create() is %d\n", rc2);
		return 0;
	}

	pthread_exit(NULL);
}
