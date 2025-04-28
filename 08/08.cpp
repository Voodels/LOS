#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

void handle_signal(int sig) {
    std::cout << "Child Process: Received signal " << sig << std::endl;
    exit(0);
}

int main() {
    pid_t pid;
    
    // Create a child process using fork()
    pid = fork();
    
    if (pid == 0) {  // Child Process
        signal(SIGUSR1, handle_signal);  // Set signal handler
        std::cout << "Child Process: Waiting for signal..." << std::endl;
        pause();  // Wait for signal
    } else if (pid > 0) {  // Parent Process
        sleep(2);  // Give the child process time to start and wait for signal
        std::cout << "Parent Process: Sending SIGUSR1 to child" << std::endl;
        kill(pid, SIGUSR1);  // Send SIGUSR1 signal to the child
        wait(NULL);  // Wait for the child process to exit
    } else {
        perror("fork failed");
        exit(1);
    }

    return 0;
}
