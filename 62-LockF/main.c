#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <fcntl.h>

int main() {
    int fd = open("testfile", O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    // Lock the file
    if (lockf(fd, F_LOCK, 0) == -1) {
        perror("lockf");
        exit(1);
    }
    printf("Parent locked the file\n");

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) { // Child process
        printf("Child trying to lock...\n");
        if (lockf(fd, F_LOCK, 0) == -1) {
            perror("child lockf");
            exit(1);
        }
        printf("Child acquired lock\n");
        lockf(fd, F_ULOCK, 0);
        exit(0);
    } else { // Parent process
        sleep(5); // Hold lock for 5 seconds
        lockf(fd, F_ULOCK, 0);
        printf("Parent released lock\n");
        wait(NULL);
    }

    close(fd);
    return 0;
}