//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include "TCP_Server.h"
#include <stdio.h>
#include <pthread.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define SEND_BUF_SIZE_MAX		1024
#define PORT_NO				12345
//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================
TCP_SERVER tcpServer;

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
    //char sendBuf[SEND_BUF_SIZE_MAX + 1]	= "";
    pthread_t thread;
    pthread_attr_t attr;
    
    
    /* Init TCP Server */
    retVal = TCP_SERVER_Init(&tcpServer, PORT_NO);
    
    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    pthread_create(&thread, &attr, i_Recv_Thread, (void *)&tcpServer); 
    
    if (TRUE == retVal)
    {
	  printf("Init successful\n");
	  
	  while (TRUE)
	  {
	      //fgets(sendBuf, SEND_BUF_SIZE_MAX, stdin);
	      //TCP_SERVER_Send_Packet(&tcpServer, (unsigned char *)sendBuf);
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
    TCP_SERVER *pTcp = NULL;  
   
    pTcp = (TCP_SERVER *)pData;
    
    while (TRUE)
    {
	TCP_SERVER_Recv_Packet_From_Conn(pTcp, 100);    
    }
    
    return 0;
}
