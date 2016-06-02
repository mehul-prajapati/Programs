#include <stdio.h>
#include <stdlib.h>

void *my_calloc(int num, int size);

int main (void)
{
	my_calloc(5, 2);


	return 0;
}


void *my_calloc(int num, int size)
{
	void *pRet = NULL;

	pRet = malloc(num * size);

	if (pRet == NULL)
	{
		printf("Memory allocation failed\n");
	}
	else
	{
		printf("Yeah!!! We succeeded\n");
	}

	return (pRet);
}
