#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main() {
    // Key for shared memory (must be unique)
    key_t key = ftok("shmfile", 65); // Generate a unique key

    // Shared memory ID
    int shmid;

    // Parent process creates shared memory
    if ((shmid = shmget(key, 1024, 0666 | IPC_CREAT)) == -1) {
        perror("shmget failed");
        exit(1);
    }

    printf("Parent: Shared memory created with ID: %d\n", shmid);

    // Fork the first child process
    pid_t pid1 = fork();

    if (pid1 == 0) {
        // Child Process 1: Attach, write, and detach
        char *shared_memory = (char *)shmat(shmid, NULL, 0);
        if (shared_memory == (char *)-1) {
            perror("shmat failed in Child 1");
            exit(1);
        }

        printf("Child 1: Attached to shared memory.\n");

        // Write data to shared memory
        const char *message = "Hello from Child 1!";
        strcpy(shared_memory, message);
        printf("Child 1: Wrote '%s' to shared memory.\n", message);

        // Detach from shared memory
        if (shmdt(shared_memory) == -1) {
            perror("shmdt failed in Child 1");
            exit(1);
        }

        printf("Child 1: Detached from shared memory.\n");
        exit(0); // Exit child process
    } else {
        // Wait for Child 1 to finish
        wait(NULL);

        // Fork the second child process
        pid_t pid2 = fork();

        if (pid2 == 0) {
            // Child Process 2: Attach, read, and detach
            char *shared_memory = (char *)shmat(shmid, NULL, 0);
            if (shared_memory == (char *)-1) {
                perror("shmat failed in Child 2");
                exit(1);
            }

            printf("Child 2: Attached to shared memory.\n");

            // Read data from shared memory
            printf("Child 2: Read '%s' from shared memory.\n", shared_memory);

            // Detach from shared memory
            if (shmdt(shared_memory) == -1) {
                perror("shmdt failed in Child 2");
                exit(1);
            }

            printf("Child 2: Detached from shared memory.\n");
            exit(0); // Exit child process
        } else {
            // Parent Process: Wait for Child 2 to finish
            wait(NULL);

            // Delete shared memory
            if (shmctl(shmid, IPC_RMID, NULL) == -1) {
                perror("shmctl failed in Parent");
                exit(1);
            }

            printf("Parent: Shared memory deleted.\n");
        }
    }

    return 0;
}