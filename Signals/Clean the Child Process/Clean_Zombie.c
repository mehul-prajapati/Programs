//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
//==================================================================
//================= @DEFINES =======================================
//==================================================================

//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================
sig_atomic_t child_exit_status;

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
static void clean_up_child_process (int signal_number);

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
int main (void)
{
	int child_pid  = 0;
	
	/* Handle SIGCHLD by calling clean_up_child_process. */
	struct sigaction sigchld_action;

	/* Init */
	memset (&sigchld_action, 0, sizeof (sigchld_action));

	/* Assign the Signal handling fucntion */	
	sigchld_action.sa_handler = &clean_up_child_process;
	
	/* Action when singal arrives */
	sigaction (SIGCHLD, &sigchld_action, NULL);

	child_pid = fork();

	if (child_pid > 0)
	{
		printf("Child process id :%d\n", child_pid);
	}
	else
	{
		/* Exit child process immediately */
		exit(0);
	}

	sleep(30);

	printf("Parent process exiting\n");

	return 0;
}

void clean_up_child_process (int signal_number)
{
	/* Clean up the child process. */
	int status;

	wait (&status);

	/* Store its exit status in a global variable. */
	child_exit_status = status;
}




