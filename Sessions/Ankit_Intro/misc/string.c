#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <string.h>

main()
{
char name[20];
printf("Enter your name:");
scanf("%[^\n]s",name);

	while(1)
	{
	display(name);
	}
}

display(char *p)
{

printf("You entered : %s\n", p);

}
