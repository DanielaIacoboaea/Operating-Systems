#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

sem_t s; 

void *child(void *arg) {
    sleep(1);
    printf("child\n");
    // use semaphore here
    Sem_post(&s);
    return NULL;
}

void *child_2(void *arg) {
    sleep(3);
    printf("child_2\n");
    // use semaphore here
    Sem_post(&s);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p, p2;
    printf("parent: begin\n");
    // init semaphore here
    Sem_init(&s, 0);
    Pthread_create(&p, NULL, child, NULL);
    Pthread_create(&p2, NULL, child_2, NULL);
    // use semaphore here
    Pthread_join(p, NULL);
    Pthread_join(p2, NULL);
    Sem_wait(&s);
    printf("parent: end\n");
    return 0;
}

