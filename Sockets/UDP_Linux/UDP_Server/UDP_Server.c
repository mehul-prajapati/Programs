//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "UDP_Server.h"

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define RCV_BUF_LEN_MAX				1024

//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
unsigned char UDP_SERVER_Init(UDP_SERVER *pUdpServer, int portNo)
{
    int retVal = 0;
  
    /* Validate paratmeters */
    if ((NULL == pUdpServer)
    || ((portNo > 0) && (portNo <= 1024)))
    {
	return (FALSE);
    }
  
    memset(pUdpServer, 0x00, sizeof(UDP_SERVER));
    
    /* Create a socket */
    pUdpServer->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  
    if (pUdpServer->sockfd == -1)
    {
	printf("ERR: Socket creation failed !!!");
	
	return (FALSE);
    }
  
    /* Set Socket to Non - Blocking */
    retVal = fcntl(pUdpServer->sockfd, F_SETFL, O_NONBLOCK);
  
    if (retVal == -1)
    {
	printf("ERR: Socket non blocking failed !!!");
	
	return (FALSE);
    }
  
    /* Fill up INET (Ipv4) structure parameters */
    pUdpServer->servAddr.sin_family 	 = AF_INET;
    pUdpServer->servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    pUdpServer->servAddr.sin_port	 = htons(portNo);
    
    /* Bind the socket */
    retVal = bind(pUdpServer->sockfd, (struct sockaddr *)(&(pUdpServer->servAddr)), sizeof(pUdpServer->servAddr));

    if (retVal == -1)
    {
	printf("ERR: Socket binding failed !!!");
	
	return (FALSE);
    }    

    return (TRUE);
}


unsigned char UDP_SERVER_Recv_Packet(UDP_SERVER *pUdpServer, int recvBufLen, void *pRecvBuf)
{
    int retVal = 0;
    socklen_t len = sizeof(pUdpServer->cliAddr);
    //char recvBuf[RCV_BUF_LEN_MAX] = "";
    
    if (NULL == pUdpServer)
    {
	return (FALSE);
    }
    
    retVal = recvfrom(pUdpServer->sockfd, pRecvBuf, recvBufLen, 0, (struct sockaddr *)&pUdpServer->cliAddr, &len);
  
    /* Connection closed */
    if (retVal == 0)
    {
	printf("Socket Connection closed \n");
    }
    else if (retVal > 0)
    {
	/*
	printf("====================================\n");
	printf("Message: %s", pRecvBuf);
	printf("====================================\n");
	*/
	return (TRUE);
    }
    else
    {
      /* Receive failed */ 
    }

    return (FALSE);
}


unsigned char UDP_SERVER_Send_Packet(UDP_SERVER *pUdpServer, unsigned char *pSendBuf)
{
    int retVal = 0;
  
    if ((NULL == pUdpServer)
    || (NULL == pSendBuf))
    {
      return (FALSE);
    }
      
    retVal = sendto(pUdpServer->sockfd, pSendBuf, strlen((char *)pSendBuf), 0, (struct sockaddr *)&pUdpServer->cliAddr, sizeof(pUdpServer->cliAddr));
    
    if (retVal == -1)
    {
	printf("ERR: Socket msg sending failed !!!");
	
	return (FALSE);
    }

    printf("%d bytes sent\n", retVal);

    return (TRUE);
}

unsigned char UDP_SERVER_Close_Socket(UDP_SERVER *pUdpServer)
{
    if (NULL == pUdpServer)
    {
      return (FALSE);
    }

    if (-1 == close(pUdpServer->sockfd))
    {
	printf("ERR: Socket closing failed !!!");
	
	return (FALSE);
    }

    return (TRUE);
}










