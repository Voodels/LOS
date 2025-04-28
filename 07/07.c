#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) { // Child
        printf("Child (PID: %d) started\n", getpid());
        sleep(2);
        printf("Child exiting with code 42\n");
        _exit(42); // Use _exit to avoid flushing buffers
    } else { // Parent
        printf("Parent (PID: %d) waiting for child\n", getpid());
        int status;
        wait(&status);
        if (WIFEXITED(status)) {
            printf("Child exited with code: %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}