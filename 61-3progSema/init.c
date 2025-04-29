#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
    int val;               /* Value for SETVAL */
    struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* Array for GETALL, SETALL */
};

int main() {
    key_t key;
    int semid;
    union semun arg;
    
    // Generate a key for the semaphore
    key = ftok("/tmp", 'S');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    
    // Create a semaphore set with 1 semaphore
    semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }
    
    // Initialize semaphore value to 1 (binary semaphore)
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl: SETVAL");
        exit(EXIT_FAILURE);
    }
    
    printf("Semaphore initialized with ID: %d\n", semid);
    printf("Use this ID in the P and V operation programs.\n");
    
    return 0;
}