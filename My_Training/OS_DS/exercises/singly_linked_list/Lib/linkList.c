//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <string.h>
#include <stdio.h>
#include "linkList.h"
#include <stdlib.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================

//==================================================================
//================= @ENUMS =========================================
//==================================================================
enum
{
	FIND_LAST_NODE			= 0,
	FIND_PREV_NODE			= 1,
	FIND_NODE_WITH_INPUT_VALUE	= 2
};

//==================================================================
//================= @DATATYPES =====================================
//==================================================================
LINK_LIST_NODE *pHead = NULL;

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
UINT32 i_Node_Count(LINK_LIST_NODE *pHead);
BOOL i_Find_Node(LINK_LIST_NODE *pHead, UINT8 whichNode, UINT32 value, LINK_LIST_NODE **pNode);

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
void LINK_LIST_Show_Data(void)
{
	LINK_LIST_NODE *pNode = NULL;
	UINT32 nodeCnt = 1;

	for (pNode = pHead; pNode != NULL; pNode = pNode->pNext, nodeCnt++)
	{
		printf("Node %d has value %d\n", nodeCnt, pNode->myValue);
	}

}

BOOL LINK_LIST_Add_Node(UINT32 value, UINT8 where, UINT32 nodeData)
{
	LINK_LIST_NODE *pNode = NULL;
	LINK_LIST_NODE *pTail = NULL;
	LINK_LIST_NODE *pMidNode = NULL;
	LINK_LIST_NODE *pNextNode = NULL;

	switch (where)
	{
		case LINK_LIST_ADD_NODE_AT_START:
			
			/* Ohh! no node is there in the list */
			if (NULL == pHead)
			{
				/* Let's add the node */
				pHead = calloc(1, sizeof(LINK_LIST_NODE));

				if (NULL == pHead)
				{
					printf("ERR: No memory available in the system\n");

					return (FALSE);
				}

				/* Save user data */
				pHead->myValue = value;
				pHead->pNext = NULL;
			}
			else
			{
				pNode = calloc(1, sizeof(LINK_LIST_NODE));

				if (NULL == pNode)
				{
					printf("ERR: No memory available in the system\n");

					return (FALSE);
				}

				/* Save user data */
				pNode->myValue = value;
				
				/* Move head node to this newly created node */
				pNode->pNext = pHead;
				pHead = pNode;
			}
			break;

		case LINK_LIST_ADD_NODE_AT_END:

			/* Ohh! no node is there in the list */
			if (NULL == pHead)
			{
				pHead = calloc(1, sizeof(LINK_LIST_NODE));

				if (NULL == pHead)
				{
					printf("ERR: No memory available in the system\n");

					return (FALSE);
				}

				/* Save user data */
				pHead->myValue = value;
				pHead->pNext = NULL;
			}
			else
			{
				pNode = calloc(1, sizeof(LINK_LIST_NODE));

				if (NULL == pNode)
				{
					printf("ERR: No memory available in the system\n");

					return (FALSE);
				}

				/* Save user data */
				pNode->myValue = value;
					
				/* Now we need tail node */
				if (TRUE == i_Find_Node(pHead, FIND_LAST_NODE, 0, &pTail))
				{
					pTail->pNext = pNode;
					pNode->pNext = NULL;
				}
				else
				{
					printf("Tail node not found\n", nodeData);
					return (FALSE);
				}

			}
			break;

		case LINK_LIST_ADD_NODE_AT_MIDDLE:
			/* Ohh! no node is there in the list */
			if (NULL == pHead)
			{
				pHead = calloc(1, sizeof(LINK_LIST_NODE));

				if (NULL == pHead)
				{
					printf("ERR: No memory available in the system\n");

					return (FALSE);
				}

				/* Save user data */
				pHead->myValue = value;
				pHead->pNext = NULL;
			}
			else
			{
				pNode = calloc(1, sizeof(LINK_LIST_NODE));

				if (NULL == pNode)
				{
					printf("ERR: No memory available in the system\n");

					return (FALSE);
				}
				
				if (TRUE == i_Find_Node(pHead, FIND_NODE_WITH_INPUT_VALUE, nodeData, &pMidNode))
				{
					/* Save next node */
					pNextNode = pMidNode->pNext;
					
					/* Now add after Mid node */
					pMidNode->pNext = pNode;
					pNode->pNext = pNextNode;
					pNode->myValue = value;
				}
				else
				{
					printf("Node having %d value not found\n", nodeData);
					return (FALSE);
				}
			}
			break;

		default:
			break;
	}

	printf("Node successfully added with a value %d. cheers ;)\n", value);

	return (TRUE);
}

BOOL LINK_LIST_Del_Node(UINT32 value)
{
	LINK_LIST_NODE *pNode = NULL;
	LINK_LIST_NODE *pPrevNode = NULL;

	if (TRUE == i_Find_Node(pHead, FIND_NODE_WITH_INPUT_VALUE, value, &pNode))
	{
		/* It is the head node */
		if (pNode == pHead)
		{
			pHead = pHead->pNext;
			free (pNode);
			pNode = NULL;
			printf("Node having value %d deleted\n", value);
		}		
		else
		{
			/* Find previous node */
			if (TRUE == i_Find_Node(pHead, FIND_PREV_NODE, value, &pPrevNode))
			{
				pPrevNode->pNext = pNode->pNext;
				free(pNode);
				pNode = NULL;
				printf("Node having value %d deleted\n", value);
			}
		}
	}
	else
	{
		printf("Node having value %d not found\n", value);
	}

	return (TRUE);
}

BOOL LINK_LIST_Modify_Node(UINT32 oldValue, UINT32 newValue)
{
	LINK_LIST_NODE *pNode = NULL;

	if (TRUE == i_Find_Node(pHead, FIND_NODE_WITH_INPUT_VALUE, oldValue, &pNode))
	{
		/* modify link list data */
		pNode->myValue = newValue;

		return (TRUE);
	}

	return (FALSE);
}

UINT32 i_Node_Count(LINK_LIST_NODE *pHead)
{
	UINT32 nodeCnt = 0;
	LINK_LIST_NODE *pNode = NULL;

	if (NULL == pHead)
	{
		return (FALSE);
	}

	for (pNode = pHead; pNode != NULL; pNode = pNode->pNext)
	{
		/* Increment the count */
		nodeCnt++;
	}

	return (nodeCnt);
}

BOOL i_Find_Node(LINK_LIST_NODE *pHead, UINT8 whichNode, UINT32 value, LINK_LIST_NODE **pNode)
{
	LINK_LIST_NODE *pOutNode = NULL;
	LINK_LIST_NODE *pPrevNode = NULL;

	if ((NULL == pHead) || (NULL == pNode))
	{
		printf("Input pointer is NULL\n");
		return (FALSE);
	}

	for (pOutNode = pHead; pOutNode != NULL; pOutNode = pOutNode->pNext)
	{
		if (FIND_LAST_NODE == whichNode)
		{
			for (pPrevNode = pHead; pPrevNode != NULL; pPrevNode = pPrevNode->pNext)                                          
			{                                                                                                              
				if (NULL == pPrevNode->pNext)
				{
					/* Yes, we found the tail node */
					*pNode = pPrevNode;
					
					return (TRUE);					
				}
			}
		}
		else if (FIND_NODE_WITH_INPUT_VALUE == whichNode)
		{
			/* Now compare the value */
			if (pOutNode->myValue == value)
			{
				*pNode = pOutNode;

				return (TRUE);
			}
		}
		else if (FIND_PREV_NODE == whichNode)
		{
			if (NULL != pOutNode->pNext)
			{
				/* Now compare the value */
				if (pOutNode->pNext->myValue == value)
				{
					*pNode = pOutNode;

					return (TRUE);
				}
			}
		}
	}

	return (FALSE);
}
