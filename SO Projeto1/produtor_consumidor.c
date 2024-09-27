#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 9

sem_t slot;
sem_t item;

int buffer[BUFFER_SIZE];
int buffer_index = 0;

pthread_mutex_t mbuffer;


void *produtor() {
    int item_produzido;

    

    while(1) {
        item_produzido = rand() % 3;
        printf("Produtor produziu: %d\n", item_produzido);

        sem_wait(&slot);
        pthread_mutex_lock(&mbuffer);

        buffer[buffer_index] = item_produzido;

        pthread_mutex_unlock(&mbuffer);
        sem_post(&item);

        sleep(3);
    }
}

void *consumidor() {
    int item_consumido;

    while(1) {
        sem_wait(&item);
        pthread_mutex_lock(&mbuffer);

        item_consumido = buffer[buffer_index];

        buffer_index--;
        
        pthread_mutex_unlock(&mbuffer);
        sem_post(&slot);

        printf("Consumidor consumiu: %d\n", item_consumido);
        sleep(3);
    }
}

int main() {
    pthread_t produtor1, produtor2, consumidor1, consumidor2;

    pthread_mutex_init(&mbuffer, NULL);

    sem_init(&slot, 0, BUFFER_SIZE);
    sem_init(&item, 0, 0);

    pthread_create(&produtor1, NULL, produtor, NULL);
    pthread_create(&produtor2, NULL, produtor, NULL);

    pthread_create(&consumidor1, NULL, consumidor, NULL);
    pthread_create(&consumidor2,NULL, consumidor, NULL);
   
    pthread_join(produtor1, NULL);
    pthread_join(produtor2, NULL);
    pthread_join(consumidor1, NULL);
    pthread_join(consumidor2, NULL);

    sem_destroy(&slot);
    sem_destroy(&item);
    pthread_mutex_destroy(&mbuffer);
    
    return 0;
}