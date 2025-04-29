#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

// Function prototypes
void f1();
void f2();

// Semaphore operations
struct sembuf sem_op;

int main() {
    // Generate a unique key for the semaphore
    key_t key = ftok("semfile", 65);

    // Create a semaphore set with 1 semaphore
    int semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget failed");
        return 1;
    }

    // Initialize the semaphore value to 0
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;

    arg.val = 0; // Initial value of the semaphore
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl failed");
        return 1;
    }

    printf("Semaphore initialized.\n");

    // Fork a child process
    pid_t pid = fork();

    if (pid == 0) {
        // Child process: Execute f2 after waiting for the semaphore
        printf("Child process waiting for semaphore...\n");

        // Perform P operation (wait)
        sem_op.sem_num = 0;
        sem_op.sem_op = -1;
        sem_op.sem_flg = SEM_UNDO;

        if (semop(semid, &sem_op, 1) == -1) {
            perror("semop failed in child");
            return 1;
        }

        printf("Child process acquired semaphore.\n");

        // Execute f2
        f2();

        return 0;
    } else {
        // Parent process: Execute f1 and signal the semaphore
        printf("Parent process executing f1...\n");

        // Execute f1
        f1();

        // Perform V operation (signal)
        sem_op.sem_num = 0;
        sem_op.sem_op = 1;
        sem_op.sem_flg = SEM_UNDO;

        if (semop(semid, &sem_op, 1) == -1) {
            perror("semop failed in parent");
            return 1;
        }

        printf("Parent process released semaphore.\n");

        // Wait for the child process to finish
        wait(NULL);

        // Delete the semaphore
        if (semctl(semid, 0, IPC_RMID, NULL) == -1) {
            perror("semctl failed in parent");
            return 1;
        }

        printf("Semaphore deleted.\n");
    }

    return 0;
}

// Function f1: Ask for username
void f1() {
    char username[100];
    printf("Enter your username: ");
    scanf("%s", username);
    printf("Username entered: %s\n", username);
}

// Function f2: Ask for password
void f2() {
    char password[100];
    printf("Enter your password: ");
    scanf("%s", password);
    printf("Password entered: %s\n", password);
}