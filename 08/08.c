#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

void handle_signal(int sig) {
    printf("Child Process: Received signal %d\n", sig);
    exit(0);
}

int main() {
    pid_t pid;
    
    // Create a child process using fork()
    pid = fork();
    
    if (pid == 0) {  // Child Process
        signal(SIGUSR1, handle_signal);  // Set signal handler
        printf("Child Process: Waiting for signal...\n");
        pause();  // Wait for signal
    } else if (pid > 0) {  // Parent Process
        sleep(2);  // Give the child process time to start and wait for signal
        printf("Parent Process: Sending SIGUSR1 to child\n");
        kill(pid, SIGUSR1);  // Send SIGUSR1 signal to the child
        wait(NULL);  // Wait for the child process to exit
    } else {
        perror("fork failed");
        exit(1);
    }

    return 0;
}
