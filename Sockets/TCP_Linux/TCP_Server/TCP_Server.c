//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include "TCP_Server.h"

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
static void *i_Recv_Packet(void *pArg);

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
unsigned char TCP_SERVER_Init(TCP_SERVER *pTcpServer, int portNo)
{
    int retVal = 0;
  
    /* Validate paratmeters */
    if ((NULL == pTcpServer)
    || ((portNo > 0) && (portNo <= 1024)))
    {
	return (FALSE);
    }
  
    /* Init all parameters to a value 0 */
    memset(pTcpServer, 0x00, sizeof(TCP_SERVER));
    
    /* Create a socket */
    pTcpServer->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  
    if (pTcpServer->sockfd == -1)
    {
	printf("ERR: Socket creation failed !!!\n");
	
	return (FALSE);
    }
  
    /* Fill up INET (IPv4) structure parameters */
    pTcpServer->servAddr.sin_family 	 = AF_INET;
    pTcpServer->servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    pTcpServer->servAddr.sin_port	 = htons(portNo);
    
    /* Bind the socket */
    retVal = bind(pTcpServer->sockfd, (struct sockaddr *)(&(pTcpServer->servAddr)), sizeof(pTcpServer->servAddr));

    if (retVal == -1)
    {
	printf("ERR: Socket binding failed !!!\n");
	close (pTcpServer->sockfd);

	return (FALSE);
    }    

    return (TRUE);
}


void TCP_SERVER_Recv_Packet_From_Conn(TCP_SERVER *pTcpServer, int recvBufLen)
{
    int newsockfd = 0;
    socklen_t len = sizeof(pTcpServer->cliAddr);
    pthread_t threadId = 0;
    FILE *pFp = NULL;

    if (NULL == pTcpServer)
    {
	return;
    }
    
    if (0 == listen(pTcpServer->sockfd, SOMAXCONN))
    {
        newsockfd = accept(pTcpServer->sockfd, (struct sockaddr *)(&pTcpServer->cliAddr), &len);

        if (newsockfd == -1)
        {
	    return;	
        }

	printf("New Client connected: Port No: %d\n", ntohs(pTcpServer->cliAddr.sin_port));

        pFp = fdopen(newsockfd, "r+");
	
	if (NULL != pFp)
	{
	   /* start thread */
	   if (0 == pthread_create(&threadId, 0, i_Recv_Packet, pFp))       
	   {
		/* don't track it */
	       pthread_detach(threadId);                        	    
	   }
	}
     }
}


static void *i_Recv_Packet(void *pArg)
{	
	/* get & convert the data */
	FILE *pFp = (FILE *)pArg;            
	char recvBuf[RCV_BUF_LEN_MAX] = "";

	/* proc client's requests */
	while (TRUE)
	{
	   if (NULL != fgets(recvBuf, sizeof(recvBuf), pFp))
	   {
 	      /* display message */
	      printf("Message from Client: %s\n", recvBuf);
	      /* Echo message */         
	      fputs(recvBuf, pFp);
	   }
	}

	/* close the client's channel */
	fclose(pFp);                   	    

	/* Terminate the thread */
	return 0;                           
}







