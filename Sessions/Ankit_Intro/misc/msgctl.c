#include <string.h>
#include <sys/msg.h> 

int main() 
{
    int msqid;
    int mykey=ftok("chardev.c", 'B');
  
    msqid = msgget((key_t)mykey, 0666 | IPC_CREAT);
    msgctl(msqid, IPC_RMID, NULL);
    return 0;
}
