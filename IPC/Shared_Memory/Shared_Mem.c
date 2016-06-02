//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define ERROR			-1

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
	int segment_id;
	char *pSharedMem;
	struct shmid_ds shmBuffer;
	int segment_size;
	int shared_segment_size = getpagesize();
	
	printf("Page size: %d\n", shared_segment_size);


	/* Allocate a shared memory segment. */
	segment_id = shmget (IPC_PRIVATE, shared_segment_size, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

	if (ERROR == segment_id)
	{
		printf("Shared memory allocation failed !!!\n");
		return (-1);
	}

	/* Attach the shared memory segment. */
	pSharedMem = (char *) shmat (segment_id, 0, 0);

	if ((void *)ERROR == pSharedMem)
	{
		printf("Shared memory attachment failed !!!\n");
		return (-1);
	}

	printf ("Shared memory attached at address %p\n", pSharedMem);
	
	/* Determine the segment’s size. */
	if (ERROR == shmctl (segment_id, IPC_STAT, &shmBuffer))
	{
		printf ("Shared memory control failed!!!\n");
		return (-1);
	}
	
	segment_size = shmBuffer.shm_segsz;
	printf ("Segment size: %d\n", segment_size);

	/* Write a string to the shared memory segment. */
	sprintf (pSharedMem, "Hello world");

	/* Detach the shared memory segment. */
	shmdt (pSharedMem);

	/* Reattach the shared memory segment, at a different address. */
	pSharedMem = (char*) shmat (segment_id, 0, 0);

	printf ("Shared memory reattached at address %p\n", pSharedMem);
	/* Print out the string from shared memory. */
	printf ("Shared memory data: %s\n", pSharedMem);

	/* Deallocate the shared memory segment. */
	if (ERROR == shmctl (segment_id, IPC_RMID, 0))
	{
		printf ("Shared memory control failed!!!\n");
		return (-1);
	}

	/* Detach the shared memory segment. */
	if (ERROR == shmdt (pSharedMem))
	{
		printf("Shared memory detachment failed !!!\n");
		return (-1);
	}

	return 0;
}


