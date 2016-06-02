#ifndef _MSG_QUE_H
#define _MSG_QUE_H

//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define MSG_BUF_SIZE_MAX			64
#define NAME_SIZE_MAX				16
#define TRUE					1
#define FALSE					0

//==================================================================
//================= @ENUMS =========================================
//==================================================================
enum
{
	MSG_QUE_KEY_1  			= 0x01,
    	MSG_QUE_KEY_2  			= 0x02,
	MSG_QUE_KEY_3  			= 0x03,
	MSG_QUE_KEY_4  			= 0x04,
	MSG_QUE_KEY_5  			= 0x05,
	MSG_QUE_KEY_6  			= 0x06,
	MSG_QUE_KEY_7  			= 0x07,
	MSG_QUE_KEY_8  			= 0x08,
	

	MSG_QUE_KEY_MAX
};

//==================================================================
//================= @DATATYPES =====================================
//==================================================================
typedef struct
{
 	long moduleId;
	char msgBuf[MSG_BUF_SIZE_MAX];
	char moduleName[NAME_SIZE_MAX];  

} IPC_MSG;


typedef struct
{
 	key_t ipcKey;
	int msgQueId;
	IPC_MSG MsgDesc;
	
} MSG_QUE;

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
unsigned char MSG_QUE_Init(MSG_QUE *pMsgQue, key_t ipcKey, long moduleId, char *pModuleName);

unsigned char MSG_QUE_Recv_Data(MSG_QUE *pMsgQue, long moduleId, char *pModuleName, char *pRecvBuf);
unsigned char MSG_QUE_Send_Data(MSG_QUE *pMsgQue, char *pSendBuf);

unsigned char MSG_QUE_Destroy(MSG_QUE *pMsgQue);



#endif
