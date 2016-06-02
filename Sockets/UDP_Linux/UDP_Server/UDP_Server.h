#ifndef _UDP_SERVER_H
#define _UDP_SERVER_H

//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <arpa/inet.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define FALSE			0
#define TRUE			1

//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================
typedef struct
{
  struct sockaddr_in servAddr;
  struct sockaddr_in cliAddr;
  
  int sockfd;
    
} UDP_SERVER;

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
unsigned char UDP_SERVER_Init(UDP_SERVER *pUdpServer, int portNo);

unsigned char UDP_SERVER_Recv_Packet(UDP_SERVER *pUdpServer, int recvBufLen, void *pRecvBuf);
unsigned char UDP_SERVER_Send_Packet(UDP_SERVER *pUdpServer, unsigned char *pSendBuf);

unsigned char UDP_SERVER_Close_Socket(UDP_SERVER *pUdpServer);
#endif
