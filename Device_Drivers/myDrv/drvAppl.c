#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int fd = 0;
	int retVal = 0;
	char buf[20] = "This is my driver";
	char readBuf[20] = "";

	fd = open("/dev/myNode", O_RDWR);
	
	if (fd < 0)
	{
		printf("failed to open the device\n");
	
		return 1;
	}

	retVal = write(fd, buf, 20);
	
        if (retVal < 0)
        {
                printf("failed to write the device\n");

                return 1;
        }

	retVal = read(fd, readBuf, 20);

        if (retVal < 0)
        {
                printf("failed to read the device\n");

                return 1;
        }

	printf("Read buf = %s\n", readBuf);

	retVal = close(fd);

	if (retVal < 0)
        {
                printf("failed to close the device\n");
       		
		return 1;
	}

	return 0;
}
