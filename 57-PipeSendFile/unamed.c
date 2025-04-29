#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main() {
    int pipe_fd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;
    int source_fd;
    
    // Create pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    // Create child process
    pid = fork();
    
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid > 0) {  // Parent process
        // Close reading end of the pipe
        close(pipe_fd[0]);
        
        // Open source file
        source_fd = open("source.txt", O_RDONLY);
        if (source_fd == -1) {
            perror("open source file");
            exit(EXIT_FAILURE);
        }
        
        printf("Parent: Sending file contents to child...\n");
        
        // Read from file and write to pipe
        while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
            bytes_written = write(pipe_fd[1], buffer, bytes_read);
            if (bytes_written == -1) {
                perror("write to pipe");
                exit(EXIT_FAILURE);
            }
        }
        
        // Close writing end of pipe and source file
        close(pipe_fd[1]);
        close(source_fd);
        
        // Wait for child to complete
        wait(NULL);
        printf("Parent: Done.\n");
    }
    else {  // Child process
        // Close writing end of the pipe
        close(pipe_fd[1]);
        
        int dest_fd = open("destination.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (dest_fd == -1) {
            perror("open destination file");
            exit(EXIT_FAILURE);
        }
        
        printf("Child: Receiving file contents from parent...\n");
        
        // Read from pipe and write to destination file
        while ((bytes_read = read(pipe_fd[0], buffer, BUFFER_SIZE)) > 0) {
            bytes_written = write(dest_fd, buffer, bytes_read);
            if (bytes_written == -1) {
                perror("write to file");
                exit(EXIT_FAILURE);
            }
        }
        
        // Close reading end of pipe and destination file
        close(pipe_fd[0]);
        close(dest_fd);
        
        printf("Child: Done receiving file.\n");
    }
    
    return 0;
}