#include<stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>


int main(void)
{
    key_t key;
    int semid;
    struct sembuf sb;

key = ftok("/semaphoreid.c", 'E');
semid = semget(key, 10, 0666 | IPC_CREAT);

printf("\n Semaphore id:%d\n",semid);

}
