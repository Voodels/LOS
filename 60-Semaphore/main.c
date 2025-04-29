#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

/*
Difference between sem.h and semaphore.h:

1. sem.h (System V semaphores):
   - Part of the older System V IPC mechanisms
   - Uses functions like semget(), semop(), semctl()
   - Requires manual initialization of sembuf structures
   - Typically global across the system

2. semaphore.h (POSIX semaphores):
   - Part of the newer POSIX standard
   - Uses functions like sem_init(), sem_wait(), sem_post()
   - More straightforward API with simpler function calls
   - Can be process-shared or process-local
   - Generally preferred in modern applications
*/

// Define sembuf shorthand for semaphore operations
union semun {
    int val;               /* Value for SETVAL */
    struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* Array for GETALL, SETALL */
};

// Function to perform P operation (wait/decrement)
void sem_wait_operation(int semid) {
    struct sembuf sb = {0, -1, 0}; // {semnum, semop, semflg}
    if (semop(semid, &sb, 1) == -1) {
        perror("semop: sem_wait_operation");
        exit(EXIT_FAILURE);
    }
}

// Function to perform V operation (signal/increment)
void sem_signal_operation(int semid) {
    struct sembuf sb = {0, 1, 0}; // {semnum, semop, semflg}
    if (semop(semid, &sb, 1) == -1) {
        perror("semop: sem_signal_operation");
        exit(EXIT_FAILURE);
    }
}

int main() {
    pid_t pid;
    key_t key;
    int semid;
    union semun arg;
    
    // Generate a key for the semaphore
    key = ftok(".", 'S');
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
    
    // Create child process
    pid = fork();
    
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid > 0) {  // Parent process
        printf("Parent process (PID: %d): Waiting to enter critical section...\n", getpid());
        
        // P operation (wait)
        sem_wait_operation(semid);
        
        printf("Parent process: Entered critical section.\n");
        
        // Critical section
        printf("Parent process: Working in critical section...\n");
        sleep(2);  // Simulate work
        
        printf("Parent process: Leaving critical section.\n");
        
        // V operation (signal)
        sem_signal_operation(semid);
        
        // Wait for child to complete
        wait(NULL);
        
        // Clean up the semaphore
        if (semctl(semid, 0, IPC_RMID) == -1) {
            perror("semctl: IPC_RMID");
            exit(EXIT_FAILURE);
        }
        
        printf("Parent process: Removed semaphore.\n");
    }
    else {  // Child process
        // Sleep to ensure parent gets first chance at semaphore
        sleep(1);
        
        printf("Child process (PID: %d): Waiting to enter critical section...\n", getpid());
        
        // P operation (wait)
        sem_wait_operation(semid);
        
        printf("Child process: Entered critical section.\n");
        
        // Critical section
        printf("Child process: Working in critical section...\n");
        sleep(2);  // Simulate work
        
        printf("Child process: Leaving critical section.\n");
        
        // V operation (signal)
        sem_signal_operation(semid);
    }
    
    return 0;
}