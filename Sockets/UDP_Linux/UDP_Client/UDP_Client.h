#ifndef _UDP_CLIENT_H
#define _UDP_CLIENT_H

//==================================================================
//================= @INCLUDES ======================================
//==================================================================
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
    UDP_CLIENT_ID_1			= 0x01,
    UDP_CLIENT_ID_2			= 0x02,
    UDP_CLIENT_ID_3			= 0x03,
    UDP_CLIENT_ID_4			= 0x04,
    UDP_CLIENT_ID_5			= 0x05,
    UDP_CLIENT_ID_6			= 0x06,
    UDP_CLIENT_ID_7			= 0x07,
    UDP_CLIENT_ID_8			= 0x08,
    
    UDP_CLIENT_ID_MAX  
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
  
} UDP_CLIENT;

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
unsigned char UDP_CLIENT_Init(UDP_CLIENT *pUdpClient, unsigned char clientId, char *pIpAddr, int portNo);
unsigned char UDP_CLIENT_Set_Callback(UDP_CLIENT *pUdpClient, void (*pRecvPacket)(unsigned char clientId, char *pRecvBuf, int recvBufLen));

void UDP_CLIENT_Recv_Packet(UDP_CLIENT *pUdpClient, int recvBufLen);
unsigned char UDP_CLIENT_Send_Packet(UDP_CLIENT *pUdpClient, unsigned char *pSendBuf);

unsigned char UDP_CLIENT_Close_Socket(UDP_CLIENT *pUdpClient);
#endif
