#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
sem_t mutex;
sem_t full;
sem_t empty;
char buffer[100];
void *producer(void *arg)
{
    int i,index=0;
    for(i=0;i<26;i++)
    {
        sem_wait(&empty);
        sem_wait(&mutex);
        buffer[index]=i+64;
        printf("producer added %c to buffer\n",buffer[index]);
        sem_post(&full);
        sem_post(&mutex);
        if(++index==10)
            index=0;
    }
}
void *consumer(void *arg)
{
    int i,index=0;
    for(i=0;i<26;i++)
    {
        sem_wait(&full);
        sem_wait(&mutex);
        printf("Consumer consumed %d\n",buffer[index]);
        sem_post(&empty);
        sem_post(&mutex);
        if(++index==10)
            index=0;
    }
}
int main()
{
    pthread_t tid1,tid2;
    sem_init(&mutex,0,1);
    sem_init(&full,0,0);
    sem_init(&empty,0,10);
    pthread_create(&tid1,NULL,producer,NULL);
    pthread_create(&tid2,NULL,consumer,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
    return 0;
}
