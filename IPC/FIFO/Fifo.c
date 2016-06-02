//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h> 

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define ERROR				-1
#define FIFO_PATH			"/tmp/fifo"
#define DATA_LEN_MAX			1024
#define STOP_STR			"stop\n"

//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
int main (void)
{
	int retVal = 0;
	int fd = 0;
	char writeData[DATA_LEN_MAX] = "";

	/* Create a FIFO */
	retVal = mkfifo(FIFO_PATH, S_IWUSR | S_IRUSR);
	
	if (ERROR == retVal)
	{
		printf("FIFO creation failed!!!\n");
	}

	/* open for writing */
	fd = open (FIFO_PATH, O_WRONLY);
	
	if (ERROR == fd)
	{
		printf("FIFO file opening failed!!!\n");
	}
	
	/* Get data from user */
	while (1)
	{
		/* Get data from user */
		fgets(writeData, DATA_LEN_MAX, stdin);

		if (0 == strcasecmp(STOP_STR, writeData))
		{
			break;
		}
			
		/* write data into FIFO */
		if (ERROR == write (fd, writeData, strlen(writeData)))
		{
			printf("fd writing failed!!!\n");
			return (-1);
		}
	}
	
	system("rm "FIFO_PATH);

	if (ERROR == close (fd))
	{
		printf("fd closing failed!!!\n");
		return (-1);
	}

	return 0;
}


