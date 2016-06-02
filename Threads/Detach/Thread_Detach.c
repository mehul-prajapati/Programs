//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdio.h>
#include <pthread.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================

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
static void *i_Child_Thread(void *pData);

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
int main(void)
{
    unsigned char retVal = 0;
    pthread_t threadId;
    pthread_attr_t attr;
    char buffer[20] = "Sample Program";

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
    retVal = pthread_create(&threadId, &attr, i_Thread, (void *)buffer);  
    
    if (0 != retVal)
    {
       printf("Thread creation failed !!!\n");
       return 1;
    }
  
    /* We're done with the attribute object, so we can destroy it */
    pthread_attr_destroy(&attr);
    
    /* The main thread is done, so we need to call pthread_exit explicitly to
    *  permit the working threads to continue even after main completes.
    */
    printf("Main: program completed. Exiting.\n");
    

    pthread_exit(0);
    //return 0;
}


static void *i_Thread(void *pData)
{
        unsigned char retVal = 0;
	pthread_t child; 
	char *pBuffer = (char *)pData;

	retVal = pthread_create(&child, 0, i_Child_Thread, (void *)pBuffer);

	if (0 != retVal)
	{
	   printf("Child Thread creation failed !!! \n");

	   return 0;
	}

	pthread_detach(child);

	printf("Hi This is thread with data: %s\n", pBuffer);

	return 0;
}


static void *i_Child_Thread(void *pData)
{
	char *pBuffer = (char *)pData;

	if (pBuffer != NULL)
		printf("Hi This is child thread with data: %s\n", pBuffer);

	return 0;
}





















