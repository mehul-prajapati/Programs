//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include "MsgQue.h"
#include <stdio.h>
#include <string.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define ERROR					-1

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
unsigned char MSG_QUE_Init(MSG_QUE *pMsgQue, key_t ipcKey, long moduleId, char *pModuleName)
{
	if ((NULL == pMsgQue)
	|| (NULL == pModuleName))
	{
		return (FALSE);
	}

	memset(pMsgQue, 0x00, sizeof(pMsgQue));

	/* Save all para */
	pMsgQue->MsgDesc.moduleId = moduleId;
	strcpy(pMsgQue->MsgDesc.moduleName, pModuleName);

	/* Create Msg Queue or Get it if it already exist */
	pMsgQue->msgQueId = msgget(ipcKey, IPC_CREAT | 0666);
	
	if (ERROR == pMsgQue->msgQueId)
	{
		printf("Message Queue Creation failed!!!!\n");
		return (FALSE);
	}

	return (TRUE);
}

unsigned char MSG_QUE_Send_Data(MSG_QUE *pMsgQue, char *pSendBuf)
{
	if ((NULL == pMsgQue)
	|| (NULL == pSendBuf))
	{
		return (FALSE);
	}
	
	/* Copy data buffer */
	strcpy(pMsgQue->MsgDesc.msgBuf, pSendBuf);

	/* Send message to Queue */
	if (ERROR == msgsnd(pMsgQue->msgQueId, &pMsgQue->MsgDesc, sizeof(IPC_MSG) - sizeof(long), 0))
	{
		printf("Message sending failed. Module = %s\n", pMsgQue->MsgDesc.moduleName);
		return (FALSE);
	}

	return (TRUE);
}


unsigned char MSG_QUE_Recv_Data(MSG_QUE *pMsgQue, long moduleId, char *pModuleName, char *pRecvBuf)
{
	ssize_t rcvdBytes = 0;

	if ((NULL == pMsgQue)
	|| (NULL == pModuleName)
	|| (NULL == pRecvBuf))
	{
		return (FALSE);
	}

	/* Receive a message from the Queue */
	rcvdBytes = msgrcv(pMsgQue->msgQueId, &pMsgQue->MsgDesc, sizeof(IPC_MSG) - sizeof(long), moduleId, 0);

	if (ERROR == rcvdBytes)
	{
		printf("Message receiving failed. Module = %s\n", pMsgQue->MsgDesc.moduleName);
		return (FALSE);
	}
	
	printf("Received %d bytes of data\n", rcvdBytes);

	strcpy(pModuleName, pMsgQue->MsgDesc.moduleName);
	strcpy(pRecvBuf, pMsgQue->MsgDesc.msgBuf); 
	
	return (TRUE);	
}


unsigned char MSG_QUE_Destroy(MSG_QUE *pMsgQue)
{
	if (NULL == pMsgQue)
	{
		return (FALSE);
	}

	if (ERROR == msgctl(pMsgQue->msgQueId, IPC_RMID, NULL))
	{
		printf("Message Queue destroy failed. Id = %d\n", pMsgQue->msgQueId);
		return (FALSE);
	}

	return (TRUE);	
}














