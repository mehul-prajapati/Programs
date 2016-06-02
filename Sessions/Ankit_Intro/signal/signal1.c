#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

volatile int count = 0;

static void hdl (int sig, siginfo_t *siginfo, void *context)
{
    count=1;
	printf ("Sending PID: %ld, UID: %ld\n",(long)siginfo->si_pid, (long)siginfo->si_uid);
}
 
int main (int argc, char *argv[])
{
	int i=0;
    struct sigaction act;
    memset (&act, '\0', sizeof(act));
 
	/* Use the sa_sigaction field because the handles has two additional parameters */
	act.sa_sigaction = &hdl;
 
	/* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
	act.sa_flags = SA_SIGINFO;
 
	if (sigaction(SIGTERM, &act, NULL) < 0) 
    {
		perror ("sigaction");
	}
    while(!count)
    {
    printf("%d\n",i++);
    sleep(1);    
    }   
	return 0;
}
