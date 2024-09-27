#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 9

int buffer[BUFFER_SIZE];
int buffer_index = 0;

pthread_mutex_t mbuffer;

pthread_cond_t slot_flag_cv;
pthread_cond_t item_flag_cv;

void *produtor() {
    int item_produzido;

    while(1) {
        item_produzido = rand() % 2;
        
        printf("Produtor produziu: %d\n", item_produzido);
        pthread_mutex_lock(&mbuffer);

        while(buffer_index == BUFFER_SIZE) {
            pthread_cond_wait(&slot_flag_cv, &mbuffer);
        }
        
        buffer[buffer_index] = item_produzido;
        buffer_index++;

        pthread_mutex_unlock(&mbuffer);
        pthread_cond_signal(&item_flag_cv);

        sleep(2);
    }
}

void *consumidor() {
    int item_consumido;

    while(1) {
        pthread_mutex_lock(&mbuffer);

        while(buffer_index == 0) {
            pthread_cond_wait(&item_flag_cv, &mbuffer);
        }

        item_consumido = buffer[buffer_index - 1];
        buffer_index--;
        
        pthread_mutex_unlock(&mbuffer);
        pthread_cond_signal(&slot_flag_cv);

        printf("Consumidor consumiu: %d\n", item_consumido);
        sleep(2);
    }
}

int main() {
    pthread_t produtor1, produtor2, consumidor1, consumidor2;

    pthread_mutex_init(&mbuffer, NULL);
    pthread_cond_init(&slot_flag_cv, NULL);
    pthread_cond_init(&item_flag_cv, NULL);

    pthread_create(&produtor1, NULL, produtor, NULL);
    pthread_create(&produtor2, NULL, produtor, NULL);

    pthread_create(&consumidor1, NULL, consumidor, NULL);
    pthread_create(&consumidor2,NULL, consumidor, NULL);
   
    pthread_join(produtor1, NULL);
    pthread_join(produtor2, NULL);
    pthread_join(consumidor1, NULL);
    pthread_join(consumidor2, NULL);
    
    return 0;
}