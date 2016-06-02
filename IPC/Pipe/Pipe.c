//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define MAX_FD			2
#define BUF_SIZE_MAX		1024
//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
void i_writer (const char *pMsg, int count, FILE *pFp);
void i_reader (FILE *pFp);

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
int main (void)
{
	int fds[MAX_FD];
	pid_t pid;
	
	/* Create a pipe. File descriptors for the two ends of the pipe are
	placed in fds. */
	pipe (fds);
	
	/* Fork a child process. */
	pid = fork ();

	if (pid == (pid_t) 0)
	{
		FILE *pFp;
	
		/* This is the child process. Close our copy of the write end of
		the file descriptor. */
		close (fds[1]);

		/* Convert the read file descriptor to a FILE object, and read
		from it. */
		pFp = fdopen (fds[0], "r");
		i_reader (pFp);

		close (fds[0]);
	}
	else 
	{
		/* This is the parent process. */
		FILE *pFp;

		/* Close our copy of the read end of the file descriptor. */
		close (fds[0]);

		/* Convert the write file descriptor to a FILE object, and write
		to it. */
		pFp = fdopen (fds[1], "w");

		i_writer ("Hello world", 5, pFp);

		close (fds[1]);
	}

	return 0;
}


void i_writer (const char* pMsg, int count, FILE *pFp)
{
	for (; count > 0; count--) 
	{
		/* Write the pMsg to the pFp, and send it off immediately. */
		fprintf (pFp, "%s\n", pMsg);
	
		fflush (pFp);
		
		/* Snooze a while. */
		sleep (1);
	}
}


/* Read random strings from the pFp as long as possible. */
void i_reader (FILE *pFp)
{
	char buffer[BUF_SIZE_MAX] = "";
	
	/* Read until we hit the end of the pFp. fgets reads until
	either a newline or the end-of-file. */
	while (!feof (pFp) && !ferror (pFp))
	{
		if (fgets(buffer, sizeof (buffer), pFp) != NULL)
		{
			fputs (buffer, stdout);
		}
	}
}






