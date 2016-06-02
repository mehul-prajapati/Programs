#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


void *doSomething1();
void *doSomething2();
void *doSomething3();

sem_t sem;

int main() 
{
int value1;    
    // initialize semaphore to 2
    sem_init(&sem, 1, 2);
    sem_getvalue(&sem, &value1);
    printf("\n \033[22;35m");
    printf("Initial sem value is : %d\n",value1);
    pthread_t thread1, thread2, thread3;

    pthread_create(&thread1, NULL, &doSomething1, NULL);
    pthread_create(&thread2, NULL, &doSomething2, NULL);
    pthread_create(&thread3, NULL, &doSomething3, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    return 0;
}

void doSomething(char c) 
{

int i,time,value;
static int t=0;

    for (i = 0; i < 3; i++)
    {
        if (sem_wait(&sem) == 0) // P operation
        {
            
            sem_getvalue(&sem, &value);
            printf("\033[22;32m sem value decremented by %c to %d\n",c,value); 
           // generate random amount of time (< 30 seconds)
            time = (int) ((double) rand() / RAND_MAX * 5);
         
            printf("\033[01;37m Thread %c enters and sleeps for %d seconds...\n", c, time);

            sleep(time);
            t+=1;
            printf("\033[01;37m Thread %c incremented counter= %d \n", c,t);
            printf("\033[01;37m Thread %c leaves the critical section\n", c);
          
            sem_post(&sem); //V operation
           
            sem_getvalue(&sem, &value);
            printf("\033[22;31m sem value incremented by %c to %d\n",c,value);
        }
    }
}

void *doSomething1() 
{
    doSomething('A');// thread A
    return 0;
}

void *doSomething2() 
{
    doSomething('B');// thread B
    return 0;
}

void *doSomething3() 
{
    doSomething('C');// thread C
    return 0;
} 
