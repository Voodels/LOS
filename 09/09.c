#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// Signal handler for the child process
void signal_handler(int sig) {
    if (sig == SIGUSR1) {
        printf("Child Process: Received SIGUSR1 signal\n");
    }
}

int main() {
    pid_t pid;

    // Create a child process using fork
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    // Child process
    if (pid == 0) {
        // Set up the signal handler
        signal(SIGUSR1, signal_handler);

        // Wait for the signal to be received
        printf("Child Process: Waiting for signal...\n");
        while(1);  // Infinite loop to keep child process alive to receive signal
    }
    // Parent process
    else {
        // Send a signal to the child process
        printf("Parent Process: Sending SIGUSR1 to child process\n");
        sleep(1);  // Ensure child is ready to receive signal
        kill(pid, SIGUSR1);

        // Wait for the child process to terminate
        wait(NULL);
    }
    return 0;
}

/*

Explanation:
Forking the Process: The fork() system call is used to create a child process. The child process gets a pid of 0, while the parent process gets the child's pid.

Signal Handling: In the child process, we set up a signal handler using signal(). The child process listens for the SIGUSR1 signal and handles it using the signal_handler() function.

Sending the Signal: The parent process sends the SIGUSR1 signal to the child using the kill() system call. The signal tells the child to handle the event and print a message.

Waiting: After sending the signal, the parent waits for the child process to finish using wait().

How it Works:
Child Process: It waits indefinitely (in a while(1) loop) to handle signals. When it receives SIGUSR1, it prints a message.

Parent Process: The parent sends SIGUSR1 to the child after a brief delay (sleep(1)). The child receives and processes the signal, then prints a message.

*/