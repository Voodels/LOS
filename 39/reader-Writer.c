#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t rw_mutex, mutex;
int read_count = 0;
int data = 0;

void *writer(void *arg) {
    sem_wait(&rw_mutex);
    data++;
    printf("Writer %d wrote: %d\n", *((int *)arg), data);
    sem_post(&rw_mutex);
    return NULL;
}

void *reader(void *arg) {
    sem_wait(&mutex);
    read_count++;
    if(read_count == 1)
        sem_wait(&rw_mutex);
    sem_post(&mutex);
    
    printf("Reader %d read: %d\n", *((int *)arg), data);
    
    sem_wait(&mutex);
    read_count--;
    if(read_count == 0)
        sem_post(&rw_mutex);
    sem_post(&mutex);
    return NULL;
}

int main() {
    pthread_t readers[5], writers[2];
    sem_init(&mutex, 0, 1);
    sem_init(&rw_mutex, 0, 1);
    
    int ids[] = {1,2,3,4,5};
    
    for(int i=0; i<5; i++)
        pthread_create(&readers[i], NULL, reader, &ids[i]);
    for(int i=0; i<2; i++)
        pthread_create(&writers[i], NULL, writer, &ids[i]);
    
    for(int i=0; i<5; i++)
        pthread_join(readers[i], NULL);
    for(int i=0; i<2; i++)
        pthread_join(writers[i], NULL);
    
    sem_destroy(&mutex);
    sem_destroy(&rw_mutex);
    return 0;
}