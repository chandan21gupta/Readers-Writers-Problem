#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>

pthread_t readers[4];
pthread_t writers[4];

int current_readers = 0;
int counter = 0;

sem_t mutex;
sem_t writer_semaphore;

void print_error(int err) {
    printf("Error : %s\n",strerror(err));
}

void *do_read(int *ptr) {

    sem_wait(&mutex);
    //pthread_mutex_lock(&reader_lock);
    int reader_number = *ptr;
    current_readers += 1;
    if(current_readers == 1) {
        //pthread_mutex_lock(&writer_lock);
        sem_wait(&writer_semaphore);
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
        sem_post(&writer_semaphore);
    }
    //pthread_mutex_unlock(&reader_lock);
    sem_post(&mutex);
    return NULL;
}

void *do_write(int *ptr) {

    //pthread_mutex_lock(&writer_lock);
    sem_wait(&writer_semaphore);
    int writer_number = *ptr;
    printf("writer %d started with : %d\n",writer_number,counter);
    counter++;
    for (unsigned long i = 0; i < (0xFFFFFFFF); i++);
    printf("writer %d finished with : %d\n",writer_number,counter);
    //pthread_mutex_unlock(&writer_lock);
    sem_post(&writer_semaphore);

    return NULL;
}

int main() {

    //pthread_mutex_init(&reader_lock,NULL);
    //pthread_mutex_init(&writer_lock,NULL);

    if((sem_init(&mutex,0,1)) < 0) {
        print_error(errno);
        exit(1);
    }

    if((sem_init(&writer_semaphore,0,1)) < 0) {
        print_error(errno);
        exit(1);
    }

    while(1){

            for(int i=0;i<4;i++) {

            if((pthread_create(&readers[i],NULL,&do_read,&i)) != 0) {
                    print_error(errno);
                    exit(1);
                }
            if((pthread_create(&writers[i],NULL,&do_write,&i)) != 0) {
                    print_error(errno);
                    exit(1);
                }
        }

        for(int i=0;i<4;i++) {
            pthread_join(writers[i],NULL);
            pthread_join(readers[i],NULL);
        }
    }
}
