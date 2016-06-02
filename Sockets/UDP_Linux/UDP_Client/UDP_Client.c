//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "UDP_Client.h"
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



//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
unsigned char UDP_CLIENT_Init(UDP_CLIENT *pUdpClient, unsigned char clientId, char *pIpAddr, int portNo)
{
    int retVal = 0;
  
    /* Validate paratmeters */
    if ((NULL == pUdpClient)
    || (NULL == pIpAddr)
    || (strlen(pIpAddr) > HOST_NAME_LEN_MAX))
    //|| ((portNo > 0) && (portNo <= 1024)))
    {
	return (FALSE);
    }
  
    memset(pUdpClient, 0x00, sizeof(UDP_CLIENT));
    
    /* Save client Id */
    pUdpClient->clientId = clientId;
  
    /* Create a socket */
    pUdpClient->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  
    if (pUdpClient->sockfd == -1)
    {
	printf("ERR: Socket creation failed !!!");
	
	return (FALSE);
    }
  
    /* Set Socket to Non - Blocking */
    retVal = fcntl(pUdpClient->sockfd, F_SETFL, O_NONBLOCK);
  
    if (retVal == -1)
    {
	printf("ERR: Socket non blocking failed !!!");
	
	return (FALSE);
    }
    
    /* Fill up INET (IPv4) structure parameters */
    pUdpClient->servaddr.sin_family 		= AF_INET;
    pUdpClient->servaddr.sin_port		= htons(portNo);    

    retVal = inet_aton(pIpAddr, &pUdpClient->servaddr.sin_addr);
    
    if (retVal == 0)
    {
	printf("ERR: inet_aton failed !!!");
	
	return (FALSE);
    }  
    
    return (TRUE);
}


unsigned char UDP_CLIENT_Set_Callback \
( \
	UDP_CLIENT *pUdpClient, \
	void (*pRecvPacket)(unsigned char clientId, char *pRecvBuf, int recvBufLen) \
)
{
    /* Validate parameters */
    if ((NULL == pUdpClient)
    || (NULL == pRecvPacket))
    {
	return (FALSE);
    }
  
    pUdpClient->pRecvPacket 	= pRecvPacket;
  
    return (TRUE);
}


void UDP_CLIENT_Recv_Packet(UDP_CLIENT *pUdpClient, int recvBufLen)
{
    char recvBuf[RCV_BUF_LEN_MAX] = "";
    int retVal = 0;
    
    if (NULL == pUdpClient)
    {
	return;
    }
    
    retVal = recvfrom(pUdpClient->sockfd, recvBuf, recvBufLen, 0, NULL, NULL);
  
    /* Data received */
    if (retVal > 0)
    {
	(*(pUdpClient->pRecvPacket))(pUdpClient->clientId, recvBuf, retVal);
    }
    else
    {
      /* Receive failed */ 
    }  
}


unsigned char UDP_CLIENT_Send_Packet(UDP_CLIENT *pUdpClient, unsigned char *pSendBuf)
{
    int retVal = 0;
  
    if ((NULL == pUdpClient)
    || (NULL == pSendBuf))
    {
      return (FALSE);
    }

    retVal = sendto(pUdpClient->sockfd, pSendBuf, strlen((char *)pSendBuf), 0, (struct sockaddr *)&pUdpClient->servaddr, sizeof(pUdpClient->servaddr));
    
    if (retVal == -1)
    {
	printf("ERR: Socket msg sending failed !!!");
	
	return (FALSE);
    }

    printf("%d bytes sent\n", retVal);
    
    return (TRUE);
}


unsigned char UDP_CLIENT_Close_Socket(UDP_CLIENT *pUdpClient)
{
    if (NULL == pUdpClient)
    {
      return (FALSE);
    }

    if (-1 == close(pUdpClient->sockfd))
    {
	printf("ERR: Socket closing failed !!!");
	
	return (FALSE);
    }

    return (TRUE);
}










