/*

*/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <string.h>


/* */
struct test_struct
{
    char name[30];
    long long no;
	char email[25];
	char addr[30];
	struct test_struct *prev;
    struct test_struct *next;
};

struct test_struct *head = NULL;
struct test_struct *curr = NULL;


struct test_struct* create_list(void)
{
    
	long long num;  

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
	
	printf("Enter Email ID:");
    scanf("%s",ptr->email);
	
	printf("Enter Address:");
    scanf(" %[^\n]s",ptr->addr);
	
    ptr->no = num;
	ptr->next = NULL;
	ptr->prev = NULL;
	head = curr = ptr;
    
	return ptr;
}

struct test_struct* add_to_list(void)
{
    
	char temp[30];
	long long num; 
	int ret;	
	
	
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
			
			ret=strcmp(ptr->name,ptr1->name);
			
			if(ret == 0)
			{	
				printf("Name already exists \n\n");
				return;
			}
			
			ptr1 = ptr1->next;
	
		}
		
	
	printf("Enter Phone Number:");
    scanf("%lld",&num);
	
	printf("Enter Email ID:");
    scanf("%25s",ptr->email);
	
	printf("Enter Address:");
    scanf(" %[^\n]s",ptr->addr);
	
	ptr->no=num;
	ptr->next = NULL;
	ptr->prev = curr;
	curr->next = ptr;
	curr=ptr;

	return ptr;
	
}

struct test_struct* search_in_list(char *tmp1, struct test_struct **prev)
{
    struct test_struct *ptr = head;
    struct test_struct *tmp = NULL;
	int ret;
    bool found = false;

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

int delete_from_list(char *tmp1)
{
    struct test_struct *tmp2 = NULL,*tmp3;
    struct test_struct *del = NULL;
	
	printf("\n Deleting name %s from list\n",tmp1);
	
    del = search_in_list(tmp1,&tmp2);

    if(del == NULL)
    {
        return -1;
    }
    else
    {
        if(tmp2 != NULL)
            
			tmp3=del->next;
			tmp2->next = tmp3;
			tmp3->prev=tmp2;

        if(del == curr)
        {
            curr = tmp2;
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
					printf("\n Search  failed, no such element found\n");
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
