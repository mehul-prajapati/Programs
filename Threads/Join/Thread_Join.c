//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdio.h>
#include <pthread.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define MAX_THREADS		4
//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
static void *i_Thread(void *pData);

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
int main(void)
{
    unsigned char retVal = 0;
    int idx = 0;
    pthread_t threadId[MAX_THREADS];

    /* Create threads */
    for (idx = 0; idx < MAX_THREADS; idx++)
    {
   	retVal = pthread_create(&threadId[idx], NULL, i_Thread, (void *)idx);  
    
	if (0 != retVal)
	{
	    printf("Thread creation failed !!!\n");
	    return 1;
	}
    }
    
    /* Make threads joinable */
    for (idx = 0; idx < MAX_THREADS; idx++)
    {
	if (!pthread_join(threadId[idx], NULL))
	{
	    printf("Thread join failed !!!\n");
	    return 1;
	}
    }

    /* The main thread is done
    */
    printf("Main: program completed. Exiting.\n");

   
    return 0;
}


static void *i_Thread(void *pData)
{
	unsigned int id = (unsigned int)pData;

	printf("This is thread with id %d\n", id);

	return 0;
}
