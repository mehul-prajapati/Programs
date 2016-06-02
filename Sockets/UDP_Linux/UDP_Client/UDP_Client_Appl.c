//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include "UDP_Client.h"
#include <stdio.h>
#include <pthread.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define SEND_BUF_SIZE_MAX		1024
#define IP_ADDR				"10.99.19.66"
#define PORT_NO				12345

//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================
UDP_CLIENT udpClient;

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
static void *i_Recv_Thread(void *pData);
static void i_Udp_Recv_Packet(unsigned char clientId, char *pRecvBuf, int recvBufLen);

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
int main(void)
{
    unsigned char retVal = 0;
    char sendBuf[SEND_BUF_SIZE_MAX + 1]	= "";
    pthread_t thread;
    pthread_attr_t attr;
    
    /* Init UDP Client */
    retVal = UDP_CLIENT_Init(&udpClient, UDP_CLIENT_ID_1, IP_ADDR, PORT_NO);
    
    if (TRUE == retVal)
    {
	retVal = UDP_CLIENT_Set_Callback(&udpClient, i_Udp_Recv_Packet);
   
	if (TRUE == retVal)
	{
	   /* Initialize and set thread detached attribute */
	   pthread_attr_init(&attr);
	   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	   if (0 == pthread_create(&thread, &attr, i_Recv_Thread, (void *)&udpClient))
	   {
	      printf("Init successful\n");

	      while (TRUE)
	      {
	         fgets(sendBuf, SEND_BUF_SIZE_MAX, stdin);
	         UDP_CLIENT_Send_Packet(&udpClient, (unsigned char *)sendBuf);
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


static void i_Udp_Recv_Packet(unsigned char clientId, char *pRecvBuf, int recvBufLen)
{
    printf("====================================\n");
    printf("UDP Server: %s", pRecvBuf);
    printf("====================================\n");
}


static void *i_Recv_Thread(void *pData)
{
    UDP_CLIENT *pUdpClient = NULL;  
   
    pUdpClient = (UDP_CLIENT *)pData;
    
    while (TRUE)
    {
	UDP_CLIENT_Recv_Packet(pUdpClient, 100);   
    }
    
    return 0;
}
