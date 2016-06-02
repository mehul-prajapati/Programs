//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define ERROR				-1
#define CPU_INFO_FILE_PATH		"/proc/cpuinfo"
#define TRUE				1
#define FALSE				0
#define BUF_SIZE_MAX			1024
#define CPU_CLOCK_SPEED_STR		"cpu MHz"

//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
static unsigned char i_Get_Cpu_Info(char *pInfoStr);

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
int main (void)
{
	unsigned char retVal = FALSE;

	retVal = i_Get_Cpu_Info(CPU_CLOCK_SPEED_STR);

	if (FALSE == retVal)
	{
		printf("ERR: cpu info getting failed !!!\n");
	}

	return 0;
}

static unsigned char i_Get_Cpu_Info(char *pInfoStr)
{
	int retVal = 0;
	float clockSpeed = 0.0;
	char *pMatch = NULL;
	char cpuInfo[BUF_SIZE_MAX] = "";
	/* Integer for file descriptor returned by open() call. */
	int fd = 0;

	/* 
	Returns the clock speed of the system’s CPU in MHz, as reported by
	/proc/cpuinfo. 
	On a multiprocessor machine, returns the speed of the first CPU. 
	On error returns zero.
	*/
	fd = open(CPU_INFO_FILE_PATH, O_RDONLY);

	/* Error occured in file opening */
	if (ERROR == fd)
	{	
		printf("ERR: File opening failed !!!\n");
		return (FALSE);
	}

	/* Read cpu info file */
	retVal = read(fd, cpuInfo, BUF_SIZE_MAX);

	/* Error occured in file opening */
	if (ERROR == retVal)
	{
		printf("ERR: File read failed !!!\n");
		return (FALSE);
	}

	/* Close the file */
	close(fd);

	/* Append terminating null character */
	cpuInfo[retVal] = '\0';

	/* Find the string */
	pMatch = strstr(cpuInfo, pInfoStr);

	if (NULL == pMatch)
	{
		printf("ERR: String search failed !!!\n");
		return (FALSE);
	}	

	sscanf(pMatch, "cpu MHz : %f", &clockSpeed);

	printf("Clock speed = %f GHz\n", clockSpeed / 1000);

	return (TRUE);
}











