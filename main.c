#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t reader_lock;
pthread_mutex_t writer_lock;

pthread_t readers[4];
pthread_t writers[4];

int current_readers = 0;
int counter = 0;

sem_t mutex;
sem_t db;

void *do_read(int *ptr) {

    sem_wait(&mutex);
    //pthread_mutex_lock(&reader_lock);
    int reader_number = *ptr;
    current_readers += 1;
    if(current_readers == 1) {
        //pthread_mutex_lock(&writer_lock);
        sem_wait(&db);
    }

    //pthread_mutex_unlock(&reader_lock);
    sem_post(&mutex);

    printf("reader %d started with counter : %d\n",reader_number,counter);
    for (unsigned long i = 0; i < (0xFFFFFFFF); i++);
    sem_wait(&mutex);
    printf("reader %d finished with counter : %d\n",reader_number,counter);

    //pthread_mutex_lock(&reader_lock);
    current_readers--;
    if(current_readers == 0) {
        //pthread_mutex_unlock(&writer_lock);
        sem_post(&db);
    }
    //pthread_mutex_unlock(&reader_lock);
    sem_post(&mutex);
}

void *do_write(int *ptr) {

    //pthread_mutex_lock(&writer_lock);
    sem_wait(&db);
    int writer_number = *ptr;
    printf("writer %d started with : %d\n",writer_number,counter);
    counter++;
    for (unsigned long i = 0; i < (0xFFFFFFFF); i++);
    printf("writer %d finished with : %d\n",writer_number,counter);
    //pthread_mutex_unlock(&writer_lock);
    sem_post(&db);
}

int main() {

    //pthread_mutex_init(&reader_lock,NULL);
    //pthread_mutex_init(&writer_lock,NULL);

    sem_init(&mutex,0,1);
    sem_init(&db,0,1);

    while(1){

            for(int i=0;i<4;i++) {

            pthread_create(&readers[i],NULL,&do_read,&i);
            pthread_create(&writers[i],NULL,&do_write,&i);

        }

        for(int i=0;i<4;i++) {
            pthread_join(writers[i],NULL);
            pthread_join(readers[i],NULL);
        }
    }
}
