#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

// Mutex declared
sem_t mutex;
sem_t full;
sem_t empty;
char buffer[100];

//MARK: - Producer function
void *producer(void *arg)
{
    int i,index=0;
    for(i=0;i<26;i++)
    {
        sem_wait(&empty);
        sem_wait(&mutex);
        buffer[index]=i+64;
        printf("producer added %c to buffer\n",buffer[index]);
        // Sem post added for semophore waiting if not added then the process will be in deadlock.
        sem_post(&full);
        sem_post(&mutex);
        if(++index==10)
            index=0;
    }
}

//MARK: - Consumer function
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

//MARK: - Main function
int main()
{
    // Initialize the pthread_t objects from the pthread.h library
    pthread_t tid1,tid2;
    // initialize the mutex for the the above declared
    sem_init(&mutex,0,1);
    sem_init(&full,0,0);
    sem_init(&empty,0,10);
    //Create pthread for producer
    pthread_create(&tid1,NULL,producer,NULL);
    //Create pthread for consumer
    pthread_create(&tid2,NULL,consumer,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    // Destroying the mutex after the using it if not required.
    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
    return 0;
}
