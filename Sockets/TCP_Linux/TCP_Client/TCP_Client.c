//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "TCP_Client.h"
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
unsigned char TCP_CLIENT_Init_And_Connect(TCP_CLIENT *pTcpClient, unsigned char clientId, char *pIpAddr, int portNo)
{
    int retVal = 0;
  
    /* Validate paratmeters */
    if ((NULL == pTcpClient)
    || (NULL == pIpAddr)
    || (strlen(pIpAddr) > HOST_NAME_LEN_MAX)
    || ((portNo > 0) && (portNo <= 1024)))
    {
	return (FALSE);
    }
  
    memset(pTcpClient, 0x00, sizeof(TCP_CLIENT));
    
    /* Save client Id */
    pTcpClient->clientId = clientId;
  
    /* Create a socket */
    pTcpClient->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  
    if (pTcpClient->sockfd == -1)
    {
	printf("ERR: Socket creation failed !!!\n");
	
	return (FALSE);
    }
    
    /* Fill up INET (IPv4) structure parameters */
    pTcpClient->servaddr.sin_family 		= AF_INET;
    pTcpClient->servaddr.sin_port		= htons(portNo);

    retVal = inet_aton(pIpAddr, &pTcpClient->servaddr.sin_addr);
    
    if (retVal == 0)
    {
	printf("ERR: inet_aton failed !!!\n");
	close(pTcpClient->sockfd);

	return (FALSE);
    }  

    /* Now connect to Server */
    retVal = connect(pTcpClient->sockfd, (struct sockaddr *)(&pTcpClient->servaddr), sizeof(pTcpClient->servaddr)); 
    
    if (retVal == -1)
    {
	printf("ERR: Socket connection failed !!!\n");
	close(pTcpClient->sockfd);
	
	return (FALSE);
    }

    /* Set Socket to Non - Blocking */
    retVal = fcntl(pTcpClient->sockfd, F_SETFL, O_NONBLOCK);
  
    if (retVal == -1)
    {
	printf("ERR: Socket non blocking failed !!!\n");
	close(pTcpClient->sockfd);
	
	return (FALSE);
    }

    return (TRUE);
}


unsigned char TCP_CLIENT_Set_Callback \
( \
	TCP_CLIENT *pTcpClient, \
	void (*pRecvPacket)(unsigned char clientId, char *pRecvBuf, int recvBufLen) , \
	void (*pCloseSocket)(unsigned char clientId) \
)
{
    /* Validate parameters */
    if ((NULL == pTcpClient)
    || (NULL == pRecvPacket)
    || (NULL == pCloseSocket))
    {
	return (FALSE);
    }
  
    pTcpClient->pRecvPacket 	= pRecvPacket;
    pTcpClient->pCloseSocket 	= pCloseSocket;
  
    return (TRUE);
}


void TCP_CLIENT_Recv_Packet(TCP_CLIENT *pTcpClient, int recvBufLen)
{
    int retVal = 0;
    char recvBuf[RCV_BUF_LEN_MAX] = "";    

    if (NULL == pTcpClient)
    {
	return;
    }
    
    retVal = recv(pTcpClient->sockfd, recvBuf, recvBufLen, 0);
  
    /* Connection closed */
    if (retVal == 0)
    {
	(*(pTcpClient->pCloseSocket))(pTcpClient->clientId);
    }
    else if (retVal > 0)
    {
	(*(pTcpClient->pRecvPacket))(pTcpClient->clientId, recvBuf, retVal);
    }
    else
    {
      /* Receive failed */ 
    }  
}


unsigned char TCP_CLIENT_Send_Packet(TCP_CLIENT *pTcpClient, unsigned char *pSendBuf)
{
    int retVal = 0;
  
    if ((NULL == pTcpClient)
    || (NULL == pSendBuf))
    {
      return (FALSE);
    }

    retVal = send(pTcpClient->sockfd, pSendBuf, strlen((char *)pSendBuf), 0);
    
    if (retVal == -1)
    {
	printf("ERR: Socket msg sending failed !!!\n");
	
	return (FALSE);
    }

    printf("%d bytes sent\n", retVal);
    
    return (TRUE);
}
