#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <fcntl.h>

int main() {
    int fd = open("testfile_flock", O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    // Lock the file
    if (flock(fd, LOCK_EX) == -1) {
        perror("flock");
        exit(1);
    }
    printf("Parent locked the file with flock\n");

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) { // Child process
        printf("Child trying to flock...\n");
        if (flock(fd, LOCK_EX) == -1) {
            perror("child flock");
            exit(1);
        }
        printf("Child acquired flock lock\n");
        flock(fd, LOCK_UN);
        exit(0);
    } else { // Parent process
        sleep(5); // Hold lock for 5 seconds
        flock(fd, LOCK_UN);
        printf("Parent released flock lock\n");
        wait(NULL);
    }

    close(fd);
    return 0;
}