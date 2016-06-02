//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include "UDP_Server.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define SEND_BUF_SIZE_MAX		1024
#define PORT_NO				12345
#define RCV_BUF_LEN_MAX                 1024

//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================
UDP_SERVER udpServer;
//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
void *i_Recv_Thread(void *pData);

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
int main(void)
{
    unsigned char retVal = 0;
    char sendBuf[SEND_BUF_SIZE_MAX + 1]	= "";
    pthread_t thread;
    pthread_attr_t attr;
    
    /* Init UDP Server */
    retVal = UDP_SERVER_Init(&udpServer, PORT_NO);
    
    if (TRUE == retVal)
    {
	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	if (0 == pthread_create(&thread, &attr, i_Recv_Thread, (void *)&udpServer)) 
    	{
	    printf("Init successful\n");

	    while (TRUE)
	    {
		fgets(sendBuf, SEND_BUF_SIZE_MAX, stdin);
		UDP_SERVER_Send_Packet(&udpServer, (unsigned char *)sendBuf);
	    }
	}
    }
  
    /* We're done with the attribute object, so we can destroy it */
    pthread_attr_destroy(&attr);
    
    /* The main thread is done, so we need to call pthread_exit explicitly to
    *  permit the working threads to continue even after main completes.
    */
    printf("Main: program completed. Exiting.\n");
  
    return 0;
}

void *i_Recv_Thread(void *pData)
{
    UDP_SERVER *pUdp = NULL;  
    char recvBuf[RCV_BUF_LEN_MAX] = "";   
    
    pUdp = (UDP_SERVER *)pData;
    
    while (TRUE)
    {
	if (UDP_SERVER_Recv_Packet(pUdp, 100, recvBuf))
	{
		printf("==========================================\n");
		printf("Message: %s", recvBuf);
		printf("==========================================\n");
	}    
    }
    
    return 0;
}
