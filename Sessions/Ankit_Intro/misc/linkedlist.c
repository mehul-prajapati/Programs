/*

*/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <string.h>

int delete=0;
/* */
struct test_struct
{
    char name[30];
    long long no;
	char email[25];
	char addr[30];
    struct test_struct *next;
};

struct test_struct *head = NULL;
struct test_struct *curr = NULL;


struct test_struct* create_list(void)
{
    
	long long num; 
	int i;	

	printf("\n creating list with headnode as \n");
    struct test_struct *ptr = (struct test_struct*)malloc(sizeof(struct test_struct));
	
    if(NULL == ptr)
    {
        printf("\n Node creation failed \n");
        return NULL;
    }

	printf("Enter Name:");
    scanf(" %[^\n]s",ptr->name);
	
	printf("Enter Number:");
    scanf("%lld",&num);
start:	
	printf("Enter Email ID:");
    scanf(" %[^\n]s",ptr->email);
	
	for(i=0;i<25;i++)
	{		
		if( (ptr->email[i]) == ' ')
		{
			printf("Invalid email address\n");
			goto start;
		}
	}
	
	printf("Enter Address:");
    scanf(" %[^\n]s",ptr->addr);
	
    ptr->no = num;
	ptr->next = NULL;
	head = curr = ptr;
    
	return ptr;
}

struct test_struct* add_to_list(void)
{
    
	char temp[30];
	long long num; 
	int ret,ret1,i;	
	
	struct test_struct *tmp;
	
	if(NULL == head)
    {
        return (create_list());
    }

    struct test_struct *ptr = (struct test_struct*)malloc(sizeof(struct test_struct));
    
	if(NULL == ptr)
    {
        printf("\n Node creation failed \n");
        return NULL;
    }

	printf("Enter Full Name:");
    scanf(" %[^\n]s",ptr->name);
	
	struct test_struct *ptr1 = head;
	
		while(ptr1 != NULL)
		{
				
			ret=strcmp(ptr1->name,ptr->name);
			
			if(ret == 0)
			{	
				printf("In if ret == 0 loop\n");
				printf("Name already exists \n\n");
				return;
			}
			else if (ret > 0)
			{
				if(ptr1 == head)
				{
					printf("In if ptr1==head loop\n");
					ptr->next=ptr1;
					//curr->next = ptr;
					head =ptr;
					break;
				}
				else
				{
					printf("In else ptr1==head loop\n");
					tmp->next = ptr;
					ptr->next = ptr1;
					break;
				}	
			}
			else 
			{
				printf("In els ret == 0 loop\n");
				tmp = ptr1;
				ptr1 = ptr1->next;
				
				if(ptr1 == NULL)
				{
					printf("In if ptr1 == null loop\n");
					tmp->next=ptr;
					ptr->next=NULL;
					break;
				}
			}	
		}
			
	printf("Enter Phone Number:");
    scanf("%lld",&num);
jump:	
	printf("Enter Email ID:");
    scanf(" %[^\n]s",ptr->email);
	
	for(i=0;i<25;i++)
	{
		if( (ptr->email[i]) == ' ')
		{
			printf("Invalid email address\n");
			goto jump;
		}
	}
	
	printf("Enter Address:");
    scanf(" %[^\n]s",ptr->addr);
	
	ptr->no=num;	
}

struct test_struct* search_in_list(char *tmp1, struct test_struct **prev)
{
    struct test_struct *ptr = head;
    struct test_struct *tmp = NULL;
	int ret,i=0;
    bool found = false;

	if(delete == 1)
	{
		while(ptr != NULL)
		{
			ret=strcmp(ptr->name,tmp1);
			
			if(ret == 0)
			{
				found = true;
				break;
			}
			else
			{
				tmp = ptr;
				ptr = ptr->next;
			}
		}

		if(true == found)
		{
			if(prev)
				*prev = tmp;		
		
			return ptr;
		}
		else
		{
			return NULL;
		}
	}	
	
	else
	{
		while(ptr != NULL)
		{
			if(ptr->name[0] == tmp1[0])
			{
				printf("In if loop");
				found = true;
				tmp = ptr;
				
				
				if(true == found)
				{
					printf("In true==found loop");
			
					if(prev)
						*prev = tmp;		
		
					{
						printf(" ------------------------");
						printf("\n | Name           :  %s      |\n ",ptr->name);
						printf("\n | Phone Number   :  %lld    |\n ",ptr->no);
						printf("\n | Email ID       :  %s      |\n ",ptr->email);
						printf("\n | Address        :  %s      |\n ",ptr->addr);
						printf("-------------------------\n");
				
					}
				}
				
				ptr = ptr->next;
			}
			else
			{
				printf("In else loop");
				tmp = ptr;
				ptr = ptr->next;
			}
		}
	}
    
    
}

int delete_from_list(char *tmp1)
{
    struct test_struct *prev = NULL;
    struct test_struct *del = NULL;
	
	printf("\n Deleting name %s from list\n",tmp1);
	
	delete=1;
    del = search_in_list(tmp1,&prev);
	delete=0;
	
    if(del == NULL)
    {
        return -1;
    }
    else
    {
        if(prev != NULL)
            prev->next = del->next;

        if(del == curr)
        {
            curr = prev;
        }
        else if(del == head)
        {
            head = del->next;
        }
    }

    free(del);
    del = NULL;
	return 0;
}

void print_list(void)
{
    struct test_struct *ptr = head;

    printf("\n -------Printing list Start------- \n");
	
    while(ptr != NULL)
    {
        printf(" ------------------------");
		printf("\n | Name           :  %s      |\n ",ptr->name);
		printf("\n | Phone Number   :  %lld    |\n ",ptr->no);
		printf("\n | Email ID       :  %s      |\n ",ptr->email);
		printf("\n | Address        :  %s      |\n ",ptr->addr);
		printf("-------------------------\n");
		
        ptr = ptr->next;
    }
    printf("\n -------Printing list End------- \n");

    return;
}

int main(void)
{
    int i = 0, ret = 0;
    struct test_struct *ptr = NULL;
	char tmp[30],tmp1[30];
	while(1)
	{
		printf("\n Enter 1 to add | 2 to search | 3 to print | 4 to delete \n");
		scanf("%d",&i);

		switch(i)
		{

			case 1:
			{
				add_to_list();
				break;
			}	

			case 2:
			{
				
				printf("\n Enter name to search \n");
				scanf(" %[^\n]s",tmp);

				ptr=search_in_list(tmp,NULL);

				if(NULL == ptr)
				{
					printf("\n Search failed, no such element found\n");
				}
				else
				{
					printf("\n Search passed name          = %s\n",ptr->name);
					printf("\n Search passed phone number  = %lld\n",ptr->no);
					printf("\n Search passed email id      = %s\n",ptr->email);
					printf("\n Search passed address       = %s\n",ptr->addr);
				}
				break;
			}

			case 3:
			{
				print_list();
				break;
			}	

			case 4:
			{
				printf("\n Enter name to delete \n");
				scanf(" %[^\n]s",tmp1);
				ret=delete_from_list(tmp1);

				if(ret != 0)
				{
					printf("\n Delete failed, no such element found\n");
				}
				else
				{
					printf("Name %s deleted from list \n",tmp1);
				}
				break;
			}		
		}
	}
return 0;
}
