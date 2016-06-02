#ifndef _TCP_CLIENT_H
#define _TCP_CLIENT_H

//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define HOST_NAME_LEN_MAX			1025
#define RCV_BUF_LEN_MAX				1024
#define FALSE					0
#define TRUE					1

//==================================================================
//================= @ENUMS =========================================
//==================================================================
enum
{
    TCP_CLIENT_ID_1			= 0x01,
    TCP_CLIENT_ID_2			= 0x02,
    TCP_CLIENT_ID_3			= 0x03,
    TCP_CLIENT_ID_4			= 0x04,
    TCP_CLIENT_ID_5			= 0x05,
    TCP_CLIENT_ID_6			= 0x06,
    TCP_CLIENT_ID_7			= 0x07,
    TCP_CLIENT_ID_8			= 0x08,
    
    TCP_CLIENT_ID_MAX  
};

//==================================================================
//================= @DATATYPES =====================================
//==================================================================
typedef struct
{
  unsigned char clientId;
  
  struct sockaddr_in servaddr;
  
  int sockfd;
  
  void (*pRecvPacket)(unsigned char clientId, char *pRecvBuf, int recvBufLen);
  void (*pCloseSocket)(unsigned char clientId);
  
} TCP_CLIENT;

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
unsigned char TCP_CLIENT_Init_And_Connect(TCP_CLIENT *pTcpClient, unsigned char clientId, char *pIpAddr, int portNo);
unsigned char TCP_CLIENT_Set_Callback(TCP_CLIENT *pTcpClient, void (*pRecvPacket)(unsigned char clientId, char *pRecvBuf, int recvBufLen),\
													void (*pCloseSocket)(unsigned char clientId));

void TCP_CLIENT_Recv_Packet(TCP_CLIENT *pTcpClient, int recvBufLen);
unsigned char TCP_CLIENT_Send_Packet(TCP_CLIENT *pTcpClient, unsigned char *pSendBuf);

#endif
