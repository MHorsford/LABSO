#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h> 

pthread_mutex_t lock1, lock2;

void* processA(void* arg) {
    time_t start_time = time(NULL); 

    pthread_mutex_lock(&lock1);
    printf("Process A acquired lock 1\n");
    sleep(1);

    // Detecção de deadlock: se estiver esperando por muito tempo
    while (pthread_mutex_trylock(&lock2) != 0) {
        if (difftime(time(NULL), start_time) > 2) {
            printf("Deadlock detected! Process A forced to release lock 1\n");
            pthread_mutex_unlock(&lock1);
            return NULL;
        }
    }

    printf("Process A acquired lock 2\n");

    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);

    return NULL;
}

void* processB(void* arg) {
    time_t start_time = time(NULL); 

    pthread_mutex_lock(&lock2);
    printf("Process B acquired lock 2\n");
    sleep(1);

    // Detecção de deadlock: se estiver esperando por muito tempo
    while (pthread_mutex_trylock(&lock1) != 0) {
        if (difftime(time(NULL), start_time) > 2) {
            printf("Deadlock detected! Process B forced to release lock 2\n");
            pthread_mutex_unlock(&lock2);
            return NULL;
        }
    }

    printf("Process B acquired lock 1\n");

    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);

    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    pthread_create(&t1, NULL, processA, NULL);
    pthread_create(&t2, NULL, processB, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    return 0;
}
