#include <stdio.h>



int main()
{
	int myArr[13] = { 1, 2, 35, 2, 1, 5, 5, 2, 5, 4, 5, 6, 6 };
	int max = 0, idx, jdx, tmp;

	int test[10] = { 9 };

	printf("test val = %d\n", test[4]);



	for (idx = 1; idx < 13; idx++)
	{
		for (jdx = 0; jdx < 12; jdx++)
		{
			if (myArr[jdx] > myArr[jdx + 1])
			{
				tmp = myArr[jdx];
				myArr[jdx] = myArr[jdx + 1];
				myArr[jdx + 1] = tmp;
			}
		}
	}

	for (idx = 0; idx < 13; idx++)
	{
		printf("%d,", myArr[idx]);
	}

	return 0;
}
