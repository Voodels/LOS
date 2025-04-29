#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main() {
    key_t key;
    int semid;
    struct sembuf sb = {0, 1, 0}; // {semnum, semop, semflg}
    
    // Generate the same key as in program 1
    key = ftok("/tmp", 'S');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    
    // Get the existing semaphore ID
    semid = semget(key, 1, 0666);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }
    
    printf("Attempting to perform V operation (signal/increment) on semaphore ID: %d\n", semid);
    
    // Check current value
    int semval = semctl(semid, 0, GETVAL);
    printf("Current semaphore value: %d\n", semval);
    
    // Perform V operation (signal/increment)
    if (semop(semid, &sb, 1) == -1) {
        perror("semop: V operation");
        exit(EXIT_FAILURE);
    }
    
    semval = semctl(semid, 0, GETVAL);
    printf("V operation successful. New semaphore value: %d\n", semval);
    printf("Released critical section.\n");
    
    return 0;
}