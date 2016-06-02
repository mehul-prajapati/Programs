//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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
sig_atomic_t sigCnt = 0;

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
static void i_Signal_Count(int signalNo);
static void *i_Write_Data(void *pData);
static void i_Signal_Thread(int signalNo);

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================

int main (void)
{
	/* Create a signal action struct variable */
	struct sigaction sigAct;
	int retVal = 0;
	pthread_t threadId;
	int idx = 0;
    	pthread_attr_t attr;
        
	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	/* Create writing thread */
	retVal = pthread_create(&threadId, &attr, i_Write_Data, NULL);  
	      
	if (0 != retVal)
	{
	   	printf("Thread creation failed !!!\n");
		return (-1);
	}

	/* Init */
	memset(&sigAct, 0x00, sizeof(sigAct));

	/* Set Signal handling function */ 
	sigAct.sa_handler = i_Signal_Count;

	sigaction(SIGUSR1, &sigAct, NULL);
	
	sleep(30);

	printf("SIGUSR1 signal was received %d times\n", sigCnt);	
	
    	/* We're done with the attribute object, so we can destroy it */
	pthread_attr_destroy(&attr);


	pthread_exit(0);
}


static void i_Signal_Count(int signalNo)
{
	/* Increment the count value */
	sigCnt++;

	printf("Signal in Main\n");
}

static void i_Signal_Thread(int signalNo)
{
    printf("Signal in Thread\n");

}

static void *i_Write_Data(void *pData)
{
	int read_fd;
	int retVal;
	char readBuf[20];
	struct sigaction sigActThread; 

        /* Init */                                                                                                                           
	memset(&sigActThread, 0x00, sizeof(sigActThread));                                                                                               
	                                                                                     
	/* Set Signal handling function */                                                                                                   
	sigActThread.sa_handler = i_Signal_Thread;                                                                                         
	sigaction(SIGUSR1, &sigActThread ,NULL);

	read_fd = open("/dev/stdin", O_RDWR);
		
	if (read_fd < 0)
	{
		printf("failed to open the device\n");
				
		return 1;
	}

	printf("Opening successful\n");		

	retVal = read(read_fd, readBuf, 20);

 	if (retVal < 0)
        {
              printf("failed to read the device\n");

	      printf("errno = %d\n", errno);
	      printf("retval = %d\n", retVal); 
              return 1;
	}

	return 0;
}











