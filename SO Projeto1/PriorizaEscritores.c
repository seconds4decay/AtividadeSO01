#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

sem_t mutex, escritor_sem;
int num_leitores = 0;
int escritores_esperando = 0;

void* leitor(void* arg) {
    while (1) {
        sem_wait(&mutex);
        while (escritores_esperando > 0) {
            sem_post(&mutex);
            usleep(100);
            sem_wait(&mutex);
        }
        num_leitores++;
        if (num_leitores == 1) {
            sem_wait(&escritor_sem);
        }
        sem_post(&mutex);

        printf("Leitor %ld está lendo\n", (long)arg);
        sleep(1);

        sem_wait(&mutex);
        num_leitores--;
        if (num_leitores == 0) {
            sem_post(&escritor_sem);
        }
        sem_post(&mutex);

        sleep(1);
    }
    return NULL;
}

void* escritor(void* arg) {
    while (1) {
        sleep(3);
        
        sem_wait(&mutex);
        escritores_esperando++;
        sem_post(&mutex);

        sem_wait(&escritor_sem);

        printf("Escritor %ld está escrevendo\n", (long)arg);
        sleep(2);

        printf("Escritor %ld terminou de escrever.\n", (long)arg);

        sem_post(&escritor_sem);

        sem_wait(&mutex);
        escritores_esperando--;
        sem_post(&mutex);

        sleep(3);
    }
    return NULL;
}

int main() {
    pthread_t leitores[5], escritores[3];

    sem_init(&mutex, 0, 1);
    sem_init(&escritor_sem, 0, 1);

    for (long i = 0; i < 5; i++) {
        pthread_create(&leitores[i], NULL, leitor, (void*)i);
    }

    for (long i = 0; i < 3; i++) {
        pthread_create(&escritores[i], NULL, escritor, (void*)i);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(leitores[i], NULL);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(escritores[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&escritor_sem);

    return 0;
}
