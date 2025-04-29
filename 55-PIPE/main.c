#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[100];

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        return 1;
    }

    // Fork a child process
    pid = fork();

    if (pid > 0) { // Parent process
        close(pipefd[0]); // Close the read end

        const char *message = "Hello from parent!";
        write(pipefd[1], message, strlen(message) + 1); // Write to the pipe
        close(pipefd[1]); // Close the write end
    } else if (pid == 0) { // Child process
        close(pipefd[1]); // Close the write end

        read(pipefd[0], buffer, sizeof(buffer)); // Read from the pipe
        printf("Child received: %s\n", buffer);
        close(pipefd[0]); // Close the read end
    } else {
        perror("Fork failed");
        return 1;
    }

    return 0;
}