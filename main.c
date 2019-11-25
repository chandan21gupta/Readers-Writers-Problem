#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock;

pthread_t reader;
pthread_t writer;

int counter = 0;

void *do_read() {
    pthread_mutex_lock(&lock);

    printf("reader started\n");
    for (unsigned long i = 0; i < (0xFFFFFFFF); i++) ;
    printf("reader finished\n");

    pthread_mutex_unlock(&lock);

    return NULL;
}

void *do_write() {

    pthread_mutex_lock(&lock);

    printf("writer started with : %d\n",counter);
    counter++;
    for (unsigned long i = 0; i < (0xFFFFFFFF); i++);
    printf("writer finished with : %d\n",counter);

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main() {

    pthread_mutex_init(&lock,NULL);

    while(1){
        pthread_create(&reader,NULL,&do_read,NULL);
        pthread_create(&writer,NULL,&do_write,NULL);

        pthread_join(writer,NULL);
        pthread_join(reader,NULL);
    }
}
