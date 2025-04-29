#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main() {
    const char *fifo_name = "myfifo";
    mkfifo(fifo_name, 0666); // Create a named pipe

    pid_t pid = fork();

    if (pid > 0) { // Parent process
        int fd = open(fifo_name, O_WRONLY);
        const char *message = "Hello from parent!";
        write(fd, message, strlen(message) + 1); // Write to the FIFO
        close(fd);
    } else if (pid == 0) { // Child process
        int fd = open(fifo_name, O_RDONLY);
        char buffer[100];
        read(fd, buffer, sizeof(buffer)); // Read from the FIFO
        printf("Child received: %s\n", buffer);
        close(fd);
    } else {
        perror("Fork failed");
        return 1;
    }

    unlink(fifo_name); // Remove the named pipe
    return 0;
}