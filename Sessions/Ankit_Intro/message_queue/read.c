#include <stdio.h>
#include <sys/msg.h>

int main() 
{
int msqid;

int mykey=ftok("chardev.c", 'B');  
struct message {
    long type;
    char text[40];
               } msg;

    long msgtyp = 1;

    msqid = msgget((key_t)mykey, 0666);

    msgrcv(msqid, (void *) &msg, sizeof(msg.text), msgtyp, MSG_NOERROR | IPC_NOWAIT);
 
    printf("%s \n", msg.text);

    printf("Mykey= %d\n",mykey);

    return 0;
}
