#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{

int data_processed,data_processed1;
int file_pipes[2];
const char some_data[] = "1234567891";
const char some_data1[] = "98765";
char buffer[BUFSIZ + 1];
pid_t fork_result;

memset(buffer, '\0', sizeof(buffer));

if (pipe(file_pipes) == 0)
{
fork_result = fork();
if (fork_result == -1)
{
fprintf(stderr, "Fork Failure");
exit(EXIT_FAILURE);
}

if (fork_result == 0)
{
data_processed = read(file_pipes[0], buffer, BUFSIZ);
printf("Read %d bytes: %s\n", data_processed, buffer);
data_processed1= write(file_pipes[1], some_data1, strlen(some_data));
printf("Wrote %d bytes\n", data_processed1);

exit(EXIT_SUCCESS);
}
else
{
data_processed = write(file_pipes[1], some_data, strlen(some_data));
printf("Wrote %d bytes\n", data_processed);
data_processed1 = read(file_pipes[0], buffer, BUFSIZ);
printf("Read %d bytes: %s\n", data_processed1, buffer);
}
}
exit(EXIT_SUCCESS);
}
