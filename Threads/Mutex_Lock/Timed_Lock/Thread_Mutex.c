//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define MAX_THREADS		2
#define MAX_NO			40

//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================
/* Mutex object */
pthread_mutex_t mutexLock;

/* Array */
int ascendNo[MAX_NO] = {0};
//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
static void *i_Write_Data(void *pData);
static void *i_Print_Data(void *pData);

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
int main(void)
{
    unsigned char retVal = 0;
    int idx = 0;
    pthread_t threadId[MAX_THREADS];

    /* Initialize the mutex */
    pthread_mutex_init(&mutexLock, NULL);

    /* Create writing thread */
    retVal = pthread_create(&threadId[idx], NULL, i_Write_Data, (void *)idx);  
    
    if (0 != retVal)
    {
	printf("Thread creation failed !!!\n");
	return (-1);
    }
    
    idx++;
    /* Create printing thread */
    retVal = pthread_create(&threadId[idx], NULL, i_Print_Data, (void *)idx);  
    
    if (0 != retVal)
    {
	printf("Thread creation failed !!!\n");
	return (-1);
    }

    /* Make threads joinable */
    for (idx = 0; idx < MAX_THREADS; idx++)
    {
	if (0 != pthread_join(threadId[idx], NULL))
	{
	    printf("Thread join failed !!!\n");
	    return (-1);
	}
    }

    /* The main thread is done
    */
    printf("Main: program completed. Exiting.\n");

    pthread_mutex_destroy(&mutexLock);

    return (0);
}


static void *i_Write_Data(void *pData)
{
	unsigned int idx = (unsigned int)pData;

	/* Mutex Lock */
	pthread_mutex_lock(&mutexLock);
	printf("This is thread with no %d\n", idx);

	/* Set the values */
	for (idx = 0; idx < MAX_NO; idx++)
	{
	    ascendNo[idx] = idx;
	
	    /* Additional delay */
	    if (idx == (MAX_NO / 2))
	    {
		printf("Delay of 10sec\n");
		sleep(10);
	    }
	}

	/* Mutex Un-Lock */
	pthread_mutex_unlock(&mutexLock);

	return (0);
}



static void *i_Print_Data(void *pData)
{
	unsigned int idx = (unsigned int)pData;
    	struct timespec abs_time;
	int retVal = 0;

	/* abs_time = now + 3sec */
	clock_gettime(CLOCK_REALTIME, &abs_time);

	printf("This is thread with no %d\n", idx);

	abs_time.tv_sec += 15;

	/* Try to lock the ressource again. we shall time out in 3 seconds. */
	retVal = pthread_mutex_timedlock (&mutexLock, &abs_time);

	if (retVal != 0) 
	{
		printf("Wait timed out");
	}

	for (idx = 0; idx < MAX_NO; idx++)
	{
	    printf("Data Element = %d\n", ascendNo[idx]);
	}

	/* Mutex Un-Lock */
	pthread_mutex_unlock(&mutexLock);

	return (0);
}
















