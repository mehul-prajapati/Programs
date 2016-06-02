//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

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
	char *pArg[] = {    "ls",  	/* argv[0], the name of the program. */
			    "-l",
			    NULL 
		       };

	printf("This is main program with pid: %d\n", (int) getpid ());

	i_spawn("ls", pArg);

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













