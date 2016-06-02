#include<stdio.h>

int main(int argc, char *argv[])
{
int portno,n;
portno = atoi(argv[1]);

printf("%d\n",portno);

printf("%s\n",argv[1]);
n=htons(portno);
printf("%d\n",n);

}
