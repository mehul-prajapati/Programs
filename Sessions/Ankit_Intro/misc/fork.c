#include <stdio.h>
#include <unistd.h>

main()
{
int result;
printf("This is parent process\n");
result=fork();
if(result==0)
{
execl("/bin/ls","-l",NULL);
}
else
{
printf("Parent process still continues.....\n");
}
}
