//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include "TCP_Client.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define SEND_BUF_SIZE_MAX		1024
#define IP_ADDR				"10.103.3.141"
#define PORT_NO				12345

//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================
TCP_CLIENT tcpClient;

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
static void *i_Recv_Thread(void *pData);
static void i_Tcp_Recv_Packet(unsigned char clientId, char *pRecvBuf, int recvBufLen);
static void i_Tcp_Close_Socket(unsigned char clientId);

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
int main(void)
{
    unsigned char retVal = 0;
    char sendBuf[SEND_BUF_SIZE_MAX + 1]	= "";
    pthread_t thread;
    pthread_attr_t attr;
	    
    while (TRUE)
    {
	sleep(5);
	/* Init TCP Client */
	retVal = TCP_CLIENT_Init_And_Connect(&tcpClient, TCP_CLIENT_ID_1, IP_ADDR, PORT_NO);
	 
	/* Try to connect */
	if (TRUE == retVal)
	{
	   break;
	}
    }

     retVal = TCP_CLIENT_Set_Callback(&tcpClient, &i_Tcp_Recv_Packet, &i_Tcp_Close_Socket);

     if (TRUE == retVal)
     {
	    /* Initialize and set thread detached attribute */
	    pthread_attr_init(&attr);
	    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	    
	    if (0 == pthread_create(&thread, &attr, i_Recv_Thread, (void *)&tcpClient))
	    {
	       printf("Init successful\n");
	    
	       while (TRUE)
	       {
		   /* Get input from user */
		   fgets(sendBuf, SEND_BUF_SIZE_MAX, stdin);
		   /* Now send packet */
		   retVal = TCP_CLIENT_Send_Packet(&tcpClient, (unsigned char *)sendBuf);
		   
		   if (FALSE == retVal)
		   {
			break;
		   }
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


static void i_Tcp_Recv_Packet(unsigned char clientId, char *pRecvBuf, int recvBufLen)
{
    printf("====================================\n");
    printf("TCP Server: %s", pRecvBuf);
    printf("====================================\n");
}


static void i_Tcp_Close_Socket(unsigned char clientId)
{
    printf("Connection closed: clientId = %d\n", clientId);
}


static void *i_Recv_Thread(void *pData)
{
    TCP_CLIENT *pTcpClient = NULL;  
   
    pTcpClient = (TCP_CLIENT *)pData;
    
    while (TRUE)
    {
	TCP_CLIENT_Recv_Packet(pTcpClient, 100);   
    }
    
    return 0;
}
