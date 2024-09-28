#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

pthread_rwlock_t rwlock;
pthread_mutex_t write_mutex;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int waiting_writers = 0;
char info[100];
int message = 0;

void *reader(void* arg) {

    while(1){
        pthread_mutex_lock(&write_mutex);
  
        while (waiting_writers > 0) {
            pthread_cond_wait(&cond, &write_mutex);
        }
        pthread_mutex_unlock(&write_mutex);

        pthread_rwlock_rdlock(&rwlock);
        printf("THREAD %ld - INFO CONTENT: %s\n",(long)arg, info);
        pthread_rwlock_unlock(&rwlock);
        sleep(3);
    }
    

    return NULL;
}

void *writer(void* arg) {
    while(1){
        pthread_mutex_lock(&write_mutex);
        waiting_writers++;
        pthread_mutex_unlock(&write_mutex);

        pthread_rwlock_wrlock(&rwlock);

        sprintf(info, "%d", message);
        message++;

        pthread_rwlock_unlock(&rwlock);

        pthread_mutex_lock(&write_mutex);
        waiting_writers--;
        
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&write_mutex);
        sleep(3);
    }


    return NULL;
}

int main(void) {
    pthread_t t1, t2, t3;

    pthread_mutex_init(&write_mutex, NULL);
    pthread_rwlock_init(&rwlock, NULL);
    
    strcpy(info, "Nothing changed yet!");

    pthread_create(&t2, NULL, writer, NULL);
    pthread_create(&t1, NULL, reader, (void*)1);  
    pthread_create(&t3, NULL, reader, (void*)2);  


    

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    pthread_rwlock_destroy(&rwlock);
    pthread_mutex_destroy(&write_mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
