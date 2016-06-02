#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

volatile int count = 0;

static void hdl (int sig)
{
	count=1;
    printf ("In Handler\n");
}
 
int main (int argc, char *argv[])
{
	int i=0;
    sigset_t intmask;
    struct sigaction act;
    memset (&act, '\0', sizeof(act));
 
	
	act.sa_handler = &hdl;
    sigemptyset (&intmask);
    sigaddset (&intmask, SIGTERM);

	if (sigaction(SIGTERM, &act, NULL) < 0) 
    {
		perror ("sigaction");
	}

    sigprocmask(SIG_BLOCK, &intmask, NULL);
    perror ("sigprocmask");
    fprintf(stderr, "SIGTERM signal blocked\n");

    while(i<20)
    {
    printf("%d\n",i++);
    sleep(1);    
    } 

    sigprocmask(SIG_UNBLOCK, &intmask, NULL);
    perror ("sigprocmask");
    fprintf(stderr, "SIGTERM signal unblocked\n");

    while(!count)
    {
    printf("%d\n",i++);
    sleep(1);    
    }   

    return 0;
}
