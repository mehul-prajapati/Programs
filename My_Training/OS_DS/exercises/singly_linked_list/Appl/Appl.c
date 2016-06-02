//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include "linkList.h"
#include <stdio.h>
//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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
int main (void)
{
	UINT32 input = 0, nodeData = 0, findNode = 0, oldVal = 0;

	while (1)
	{
		printf(ANSI_COLOR_CYAN "---------- Welcome ---------------\n" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_BLUE "\n1. Adding node at start position in the linked list\n");
		printf("2. Adding node at end position in the linked list\n");
		printf("3. Adding node at middle position in the linked list\n" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_RED "4. Deleting node in the linked list\n" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_YELLOW "5. Modifying node data in the linked list\n" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_MAGENTA "6. Show all data of linked list\n" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_YELLOW "0. Exit\n" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_CYAN "\n------------------------------------\n" ANSI_COLOR_RESET);
		
		scanf("%d", &input);

		switch (input)
		{
			case 1:
				printf("Enter node data: ");
				scanf("%d", &nodeData);
				LINK_LIST_Add_Node(nodeData, LINK_LIST_ADD_NODE_AT_START, 0);
				break;

			case 2:
				printf("Enter node data: ");
				scanf("%d", &nodeData);
				LINK_LIST_Add_Node(nodeData, LINK_LIST_ADD_NODE_AT_END, 0);
				break;

			case 3:
				printf("Enter node data: ");
				scanf("%d", &nodeData);

				printf("After which node it has to be added.Please give value: ");
				scanf("%d", &findNode);

				LINK_LIST_Add_Node(nodeData, LINK_LIST_ADD_NODE_AT_MIDDLE, findNode);
				break;

			case 4:
				printf("Enter node data to be deleted: ");
				scanf("%d", &nodeData);
				LINK_LIST_Del_Node(nodeData);
				break;

			case 5:
				printf("Enter old node data: ");
				scanf("%d", &oldVal);

				printf("Enter new node data: ");
				scanf("%d", &nodeData);
				LINK_LIST_Modify_Node(oldVal, nodeData);
				break;

			case 6:
				LINK_LIST_Show_Data();
				break;

			default:
				if (input)
					printf("Wrong Input man. Try again\n");
				break;
		}

		if (0 == input) break;
	}

	return 0;
}
