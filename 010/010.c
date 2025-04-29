#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// Flag to check if signal was received
volatile sig_atomic_t received = 0;

// Child signal handler
void child_handler(int sig) {
    printf("Child: Received SIGUSR1 signal\n");
    printf("Child: Sending SIGUSR2 to parent\n");
    kill(getppid(), SIGUSR2);  // Send signal to parent
    received = 1;              // Set flag to exit loop
}

// Parent signal handler
void parent_handler(int sig) {
    printf("Parent: Received SIGUSR2 signal\n");
}

int main() {
    pid_t child_pid;

    // Set up parent's signal handler
    signal(SIGUSR2, parent_handler);

    // Create child process
    if ((child_pid = fork()) == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {    // Child process
        signal(SIGUSR1, child_handler);  // Register handler
        printf("Child: PID %d (Parent %d)\n", getpid(), getppid());
        printf("Child: Waiting for signal...\n");
        
        // Wait until signal is received
        while (!received) pause();
        
        printf("Child: Exiting\n");
        exit(EXIT_SUCCESS);
    } 
    else {                  // Parent process
        printf("Parent: Child PID %d\n", child_pid);
        sleep(1);  // Give child time to set up handler
        
        printf("Parent: Sending SIGUSR1 to child\n");
        kill(child_pid, SIGUSR1);  // Send signal to child
        
        printf("Parent: Waiting for response...\n");
        pause();  // Wait for child's response
        
        wait(NULL);  // Clean up child process
        printf("Parent: Exiting\n");
    }
    return 0;
}