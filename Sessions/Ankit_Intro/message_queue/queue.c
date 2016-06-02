#include <string.h>
#include <sys/msg.h> 

int main() 
{
int msqid;
int mykey=ftok("chardev.c", 'B');
    struct message {
                  long type;
                  char text[40];
                 } msg;
long msgtyp = 0;

    msqid = msgget((key_t)mykey, 0666 | IPC_CREAT);

    msg.type = 1;

    strcpy(msg.text, "This is message 1");
    msgsnd(msqid, (void *) &msg, sizeof(msg.text), IPC_NOWAIT);

    strcpy(msg.text, "This is message 2");
    msgsnd(msqid, (void *) &msg, sizeof(msg.text), IPC_NOWAIT);

    msg.type = 2;

    strcpy(msg.text, "This is message 1 of type 2");
    msgsnd(msqid, (void *) &msg, sizeof(msg.text), IPC_NOWAIT);
    
    return 0;
}
