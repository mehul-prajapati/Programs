//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdio.h>
#include "SysTmr.h"
#include <stdlib.h>
//==================================================================
//================= @DEFINES =======================================
//==================================================================

//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================
struct TIMER_NODE_TAG
{
	SYS_TIMER *pTimer;
	struct TIMER_NODE_TAG *pNext;
};

typedef struct TIMER_NODE_TAG TIMER_NODE;

TIMER_NODE *pHead = NULL;
//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
UINT8 i_Remove_Node_From_List(TIMER_NODE *pNode);

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
UINT8 SYS_TIMER_Init(SYS_TIMER *pTimer, UINT32 appId)
{
	if (pTimer == NULL)
	{
		return (FALSE);
	}

	pTimer->appId = appId;

	return (TRUE);
}


UINT8 SYS_TIMER_Set_Callback(SYS_TIMER *pTimer, void (*pCallback)(UINT32 appId, UINT32 appData))
{
	if ((pTimer == NULL) || (pCallback == NULL))
        {
                return (FALSE);
        }

	pTimer->pCallback = pCallback;

	return (TRUE);
}


UINT8 SYS_TIMER_Start(SYS_TIMER *pTimer, UINT32 tmrCnt, UINT32 appData)
{
	TIMER_NODE *pNode = NULL;	

	if (pTimer == NULL)
        {
                return (FALSE);
        }
	
	pTimer->appData = appData;
	pTimer->tmrCnt = tmrCnt;

	/* Add a node here */	
	
	/* This is the 1st node */
	if (pHead == NULL)
	{
		pHead = calloc(1, sizeof(TIMER_NODE));
	
		if (NULL == pHead)
		{
			printf("System Timer: Failed to allocate memory of node\n");
			
			return (FALSE);
		}
		else
		{
			pHead->pTimer = pTimer;
			pHead->pNext = NULL;
		}

	}
	else
	{
		pNode = calloc(1, sizeof(TIMER_NODE));

                if (NULL == pNode)
                {
                        printf("System Timer: Failed to allocate memory of node\n");

                        return (FALSE);
                }
                else
                {
                        pNode->pTimer = pTimer;
                        
			/* Adding node before Head node*/
			pNode->pNext = pHead;
			pHead = pNode;
                }

	}

	printf("System Timer: Successfully created the node\n");

	return (TRUE);
}


UINT8 SYS_TIMER_Stop(SYS_TIMER *pTimer)
{
        if (pTimer == NULL)
        {
                return (FALSE);
        }

	/* Find & Remove node from link list */

	return (TRUE);
}


void SYS_TIMER_Tick(void)
{
	TIMER_NODE *pNode = NULL;

	if (pHead != NULL)
	{
		for (pNode = pHead; pNode != NULL; pNode = pNode->pNext)
		{
			/* Decrement the count */
			(pNode->pTimer->tmrCnt)--;
		
			if (!pNode->pTimer->tmrCnt)
			{
				/* It got expired */
				(*(pNode->pTimer->pCallback)) (pNode->pTimer->appId, pNode->pTimer->appData);

				/* Remove this node */
				i_Remove_Node_From_List(pNode);
			}
		}

		printf("Tick\n");	
	}
}


UINT8 i_Remove_Node_From_List(TIMER_NODE *pNode)
{
	TIMER_NODE *pPrevNode = NULL;

	if (NULL == pNode)
	{
		return (FALSE);
	}

	/* Is it head ? */
	if (pHead == pNode)	// Yes
	{
		/* Update head */
		pHead = pHead->pNext;
	
		free(pNode);
	}
	else // No
	{
		/* Find the prev node */
		for (pPrevNode = pHead; (pPrevNode != NULL) && (pPrevNode->pNext != pNode); pPrevNode = pPrevNode->pNext) {}

		if (pPrevNode == NULL)
		{
			printf("System Timer: Node Does Not Exist in the list\n");
			
			return (FALSE);
		}

		pPrevNode->pNext = pNode->pNext;
	
		free (pNode);	
	}

	printf("System Timer: Successfully removed the node\n");

	return (TRUE);
}
