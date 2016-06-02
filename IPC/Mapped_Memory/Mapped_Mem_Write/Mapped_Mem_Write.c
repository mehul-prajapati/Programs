//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define FILE_LENGTH 			0x100
#define ERROR				-1

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
int main (int argc, char *argv[])
{
	int fd;
	char *pFileMem = NULL;
	
	if (argc != 2)
	{
		printf("Please provide file name in argument\n");
		return (-1);
	}

	/* Prepare a file large enough to hold an unsigned integer. */
	fd = open (argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

	if (ERROR == fd)
	{
		printf("file creation failed!!!\n");
		return (-1);
	}

	if ((off_t)ERROR == lseek (fd, FILE_LENGTH + 1, SEEK_SET))
	{
		printf("file offset failed!!!\n");
		return (-1);
	}
	
	if (ERROR == write (fd, "", 1))
	{
		printf("file write failed!!!\n");
		return (-1);
	}

	if ((off_t)ERROR == lseek (fd, 0, SEEK_SET))
	{
		printf("file offset failed!!!\n");
		return (-1);
	}

	/* Create the memory mapping. */
	pFileMem = mmap (0, FILE_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0);
	
	if ((void *)ERROR == pFileMem)
	{
		printf("file mapping failed!!!\n");
		return (-1);
	}

	if (ERROR == close (fd))
	{
		printf("file close failed!!!\n");
		return (-1);
	}

	/* Write a random integer to memory-mapped area. */
	sprintf(pFileMem, "%d\n", rand());

	/* Release the memory (unnecessary because the program exits). 
	   The munmap call is unnecessary because Linux would
	   automatically unmap the file when the program terminates.
	*/
	if (ERROR == munmap (pFileMem, FILE_LENGTH))
	{
		printf("file unmapping failed!!!\n");
		return (-1);
	}

	return 0;
}


