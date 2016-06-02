#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
 
#define SIZE 256
 
static void handler (int sig)
{
	printf ("In Handler\n");
	char buffer[SIZE];
	time_t curtime;
	struct tm *loctime;

	/* Get the current time. */
	curtime = time (NULL);

	/* Convert it to local time representation. */
	loctime = localtime (&curtime);

	/* Print it out in a nice format. */
	strftime (buffer, SIZE, "Today is %B %d.\n", loctime);
	fputs (buffer, stdout);
	strftime (buffer, SIZE, "The time is %I:%M %p.\n", loctime);
	fputs (buffer, stdout);

	return 0;			
}
 
int main ()
{
    sigset_t mask;	
    struct sigaction act;
 
	memset (&act, '\0', sizeof(act));
 
	act.sa_handler = &handler;
    act.sa_flags = 0;
    
    if (sigaction(SIGTERM, &act, NULL) < 0) 
    {
		perror ("sigaction");
	}

	sleep (10);
    printf("Exiting main\n");

    return 0;
}
