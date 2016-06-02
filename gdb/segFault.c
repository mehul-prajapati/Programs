#include <stdio.h>
#include <string.h>
#define EXCHANGE(X,Y) {int t = 0; t = X; X = Y; Y=t;}
#define MULTI_CHAR 0x65, 0x66, 0x67


int main()
{
	int a = 5, b = 6;
	char buffer[4] = "" ;
	
	
	EXCHANGE(a,b);

//	printf("a = %d b = %d\n", a, b);


//	printf("MULTI_CHAR = %d", MULTI_CHAR);
	
//	strcpy(buffer, "15");
	
	printf("buffer = %s\n", buffer[10]);

//	printf("Size of unsigned = %d\n", sizeof(unsigned));
	
	return  0;
}
