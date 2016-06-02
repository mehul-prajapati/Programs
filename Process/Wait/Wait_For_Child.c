//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <signal.h>
//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define TRUE			1
#define FALSE			0

//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
static unsigned char i_spawn(char *pName, char **pArg);
 
//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
int main (void)
{
	int child_status = 0;
	pid_t child_pid	= 0;
				
	char *pArg[] = {    "child",  	/* argv[0], the name of the program. */
			    NULL 
		       };


	printf("This is main program with pid: %d\n", (int) getpid ());

	/* Create a child process */
	i_spawn("./child", pArg);

	/* Wait for the child process to complete. */
	child_pid = wait (&child_status);
	
	if (child_pid > 0)
	{
		printf("Waiting for Child process %d completed\n", (int) child_pid);

		/* Get the exit code */
		if (WIFEXITED (child_status))
		{
			printf ("The child process exited normally with exit code %d\n", WEXITSTATUS (child_status));
		}	
		else
		{	
			printf ("The child process exited abnormally with a signal no = %d\n", WTERMSIG(child_status));
		}
	}
	else
	{
		printf("Wait failed!!!\n");
	}

	printf("Parent process exiting\n");

	return (0);
}



static unsigned char i_spawn(char *pName, char **pArg)
{
	pid_t child_pid	= 0;

	/* Creating a child process */
	child_pid = fork ();

	/* Parent process */
	if (0 != child_pid)
	{
		printf("The child process id: %d\n", (int) child_pid);
	}
	else
	{
		/* 
			Separating child process to execute a different program.
			Because exec replaces the calling program with another one, it never returns unless an
			error occurs.
		*/
		if (-1 == execvp (pName, pArg))
		{
			printf("Child process exec failed !!!\n");	
			
			return (FALSE);
		}
	}

	return (TRUE);
}













