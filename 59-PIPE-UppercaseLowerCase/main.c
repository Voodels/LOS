#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

void convert_to_lowercase(char *buffer, int size) {
    for (int i = 0; i < size; i++) {
        buffer[i] = tolower(buffer[i]);
    }
}

int main(int argc, char *argv[]) {
    int pipe_fd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    
    // Check if filename is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
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
        int source_fd = open(argv[1], O_RDONLY);
        if (source_fd == -1) {
            perror("open source file");
            exit(EXIT_FAILURE);
        }
        
        printf("Parent: Reading from file and sending to child...\n");
        
        // Read from file and write to pipe
        while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
            write(pipe_fd[1], buffer, bytes_read);
        }
        
        // Close writing end of pipe and source file
        close(pipe_fd[1]);
        close(source_fd);
        
        // Wait for child to complete
        wait(NULL);
    }
    else {  // Child process
        // Close writing end of the pipe
        close(pipe_fd[1]);
        
        printf("Child: Converting to lowercase and displaying...\n");
        
        // Read from pipe, convert to lowercase, and print to stdout
        while ((bytes_read = read(pipe_fd[0], buffer, BUFFER_SIZE)) > 0) {
            convert_to_lowercase(buffer, bytes_read);
            write(STDOUT_FILENO, buffer, bytes_read);
        }
        
        // Close reading end of pipe
        close(pipe_fd[0]);
    }
    
    return 0;
}