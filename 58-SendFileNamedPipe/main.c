#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define FIFO_NAME "./myfifo"

int main() {
    pid_t pid;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;
    int source_fd, fifo_fd;
    
    // Create the named pipe (FIFO)
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("mkfifo");
        // If FIFO already exists, that's okay
        if (errno != EEXIST) {
            exit(EXIT_FAILURE);
        }
    }
    
    // Create child process
    pid = fork();
    
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid > 0) {  // Parent process
        // Open source file
        source_fd = open("source.txt", O_RDONLY);
        if (source_fd == -1) {
            perror("open source file");
            exit(EXIT_FAILURE);
        }
        
        // Open FIFO for writing
        printf("Parent: Opening FIFO for writing...\n");
        fifo_fd = open(FIFO_NAME, O_WRONLY);
        if (fifo_fd == -1) {
            perror("open FIFO for writing");
            exit(EXIT_FAILURE);
        }
        
        printf("Parent: Sending file contents to child through FIFO...\n");
        
        // Read from file and write to FIFO
        while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
            bytes_written = write(fifo_fd, buffer, bytes_read);
            if (bytes_written == -1) {
                perror("write to FIFO");
                exit(EXIT_FAILURE);
            }
        }
        
        // Close FIFO and source file
        close(fifo_fd);
        close(source_fd);
        
        // Wait for child to complete
        wait(NULL);
        printf("Parent: Done.\n");
        
        // Remove the FIFO
        unlink(FIFO_NAME);
    }
    else {  // Child process
        int dest_fd;
        
        // Open destination file
        dest_fd = open("destination.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (dest_fd == -1) {
            perror("open destination file");
            exit(EXIT_FAILURE);
        }
        
        // Open FIFO for reading
        printf("Child: Opening FIFO for reading...\n");
        fifo_fd = open(FIFO_NAME, O_RDONLY);
        if (fifo_fd == -1) {
            perror("open FIFO for reading");
            exit(EXIT_FAILURE);
        }
        
        printf("Child: Receiving file contents from parent through FIFO...\n");
        
        // Read from FIFO and write to destination file
        while ((bytes_read = read(fifo_fd, buffer, BUFFER_SIZE)) > 0) {
            bytes_written = write(dest_fd, buffer, bytes_read);
            if (bytes_written == -1) {
                perror("write to file");
                exit(EXIT_FAILURE);
            }
        }
        
        // Close FIFO and destination file
        close(fifo_fd);
        close(dest_fd);
        
        printf("Child: Done receiving file.\n");
    }
    
    return 0;
}