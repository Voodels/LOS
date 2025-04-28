#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Child Process
        printf("Child: Opening Calculator...\n");
        execlp("gnome-calculator", "gnome-calculator", NULL);
        // If execlp fails
        perror("execlp");
    } else if (pid > 0) {
        // Parent Process
        wait(NULL);
        printf("Parent: Child finished.\n");
    } else {
        // Fork Failed
        perror("fork");
    }

    return 0;
}