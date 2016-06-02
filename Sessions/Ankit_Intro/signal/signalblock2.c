#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#define SIG_STOP_CHILD SIGRTMIN+1
volatile int count = 0,count1=0;

static void hdl (int sig)
{
	count=1;
    count1++;
    printf ("In Handler\n");
}
 
int main (int argc, char *argv[])
{
	int i=0,j=0;
    sigset_t intmask;
    struct sigaction act;
    memset (&act, '\0', sizeof(act));
 
	
	
    act.sa_handler = &hdl;
    sigemptyset (&intmask);
    sigaddset (&intmask, SIG_STOP_CHILD);

	if (sigaction(SIG_STOP_CHILD, &act, NULL) < 0) 
    {
		perror ("sigaction");
	}

    sigprocmask(SIG_BLOCK, &intmask, NULL);
    perror ("sigprocmask");
    fprintf(stderr, "SIGTERM signal blocked\n");

    while(i<20)
    {
    printf("i=%d\n",i++);
    sleep(1);    
    } 

    sigprocmask(SIG_UNBLOCK, &intmask, NULL);
    perror ("sigprocmask");
    fprintf(stderr, "SIGTERM signal unblocked\n");

    while(20<i<40)
    {
    printf("i=%d\n",i++);
    sleep(1);    
    }   

    while(count1<2)
    {
    printf("j=%d\n",j++);
    sleep(1);    
    }   

    return 0;
}
