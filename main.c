#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t reader_lock;
pthread_mutex_t writer_lock;

pthread_t readers[4];
pthread_t writer[4];
int shared_data[] = {0,0,0,0,0,0,0,0};

int main() {

    pthread_mutex_init(&reader_lock);
    pthread_mutex_init(&writer_lock);
    pthread_create(&readers[0],NULL,&do_read,NULL);
    pthread_create(&writers[0],NULL,&do_write,NULL);
}
