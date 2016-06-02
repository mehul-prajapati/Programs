//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//==================================================================
//================= @DEFINES =======================================
//==================================================================

//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================
#define COMMAND_LENGTH_MAX 		1024
#define STR_LENGTH_MAX 			128

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
int main(void)
{
	char cmdLine[COMMAND_LENGTH_MAX + 1] = "";
	char cmdPrompt[STR_LENGTH_MAX + 1] = "";
	char usrName[STR_LENGTH_MAX + 1] = "";
	char hostName[STR_LENGTH_MAX + 1] = "";


	while (1)
	{	
		/* get current working directory */
		getcwd(cmdPrompt, STR_LENGTH_MAX);
		
		/* get login name */
		getlogin_r(usrName, STR_LENGTH_MAX);

		/* get host name */
		gethostname(hostName, STR_LENGTH_MAX);

		printf("%s@%s:%s$ ", usrName, hostName, cmdPrompt);

		if (!fgets(cmdLine, COMMAND_LENGTH_MAX, stdin)) break;

		system(cmdLine);
	}

	return 0;
}
