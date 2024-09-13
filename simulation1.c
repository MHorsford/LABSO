#include <stdio.h>
#include <pthread.h>
#include <unistd.h>  

pthread_mutex_t lock1, lock2;

void* processA(void* arg) {
    pthread_mutex_lock(&lock1); // Tenta adquirir o recurso 1
    printf("Process A acquired lock 1\n");
    sleep(1); // Simula algum processamento
    
    pthread_mutex_lock(&lock2); // Tenta adquirir o recurso 2
    printf("Process A acquired lock 2\n");

    // Libera os recursos
    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);

    return NULL;
}

void* processB(void* arg) {
    pthread_mutex_lock(&lock2); // Tenta adquirir o recurso 2
    printf("Process B acquired lock 2\n");
    sleep(1); // Simula algum processamento
    
    pthread_mutex_lock(&lock1); // Tenta adquirir o recurso 1
    printf("Process B acquired lock 1\n");

    // Libera os recursos
    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);

    return NULL;
}

int main() {
    pthread_t t1, t2;
    
    // Inicializa os mutexes
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    // Cria dois processos (threads)
    pthread_create(&t1, NULL, processA, NULL);
    pthread_create(&t2, NULL, processB, NULL);

    // Espera os dois processos finalizarem
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Destroi os mutexes
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    return 0;
}
