#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "MYFIFO"

int main(void)
{
    char s[300]="HI HELLO",data;
    int num, fd;

    mknod(FIFO_NAME, S_IFIFO | 0666, 0);

    printf("waiting for readers...\n");
    fd = open(FIFO_NAME, O_WRONLY);
    printf("got a reader--type some stuff\n");

    //data=gets(s); 
    {
        if ((num = write(fd, s, strlen(s))) == -1)
            perror("write");
        else
            printf("I wrote %d bytes\n", num);


		if ((num = write(fd, s, strlen(s))) == -1)
            perror("write");
        else
            printf("I wrote %d bytes\n", num);

    }

    return 0;
}
