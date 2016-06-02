#include <stdio.h>
#include <stdlib.h>

#define SUM(X,Y)		X + Y

//static unsigned char i_Calc_Checksum(unsigned char *pData, int len);

struct abc
{
	int a;
	int b;
};


int main(void)
{
	unsigned char binary[32] = "";
	int idx = 0, cnt;
	int num = 0;

	printf("sum = %d\n", SUM(4,5));

	printf("Enter the decimal no:");
	scanf("%d", &num);


	while (num > 0) 
	{
		binary[idx++] = num % 2;
		num /= 2;
	}

	for (cnt = idx; cnt >= 0; cnt--)
	{
		printf("%x", binary[cnt]);
	}

	printf("\n");

	return 0;
}
#if 0

	unsigned char rData[4] = { 0x50, 0x0D, 0x31, 0x00 };
	int x = 3,y = 4,z;
	struct abc mData;
	struct abc *pTest = &mData;
	int expr = 0;
	//	z = y = 6;

	y *= x + x;
	mData.a = 9;

//	printf("y = %d z = %d\n", y,z);

	printf("Hello Entrepreneur\n");

	printf("Checksum = %x\n", i_Calc_Checksum(rData, 4));


	if (!expr)
	{
		printf("This is expr 0\n");
	}

//	printf("a = %d\n", pTest->a);

	return 0;
}


static unsigned char i_Calc_Checksum(unsigned char *pData, int len)
{
	int checksum = 0;
	
	while (len--)
	{
		checksum += *pData++; 
	}

	// 2's
	checksum = ~checksum + 1;

	return checksum;
}

#endif









