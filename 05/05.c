#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
/*
    @<stdlib.h>
    The stdlib.h header file provides functions for performing general utility functions such as memory allocation, process control, conversions, and random number generation.
    It includes functions like malloc(), free(), atoi(), atof(), rand(), and exit().
    It also defines macros for constants like NULL, EXIT_SUCCESS, and EXIT_FAILURE.

*/
int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) { // Child process
        printf("Child (PID: %d) started\n", getpid());
        sleep(2); // Simulate work
        printf("Child (PID: %d) exiting\n", getpid());
        exit(42); // Exit with status 42
    } else { // Parent process
        printf("Parent (PID: %d) waiting for child (PID: %d)...\n", getpid(), pid);
        int status;
        wait(&status); // Wait for child and capture status
        if (WIFEXITED(status)) {
            printf("Child exited with code: %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}
