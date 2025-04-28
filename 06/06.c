#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t pid;
    int status;
    char *args[] = {"ls", "-l", NULL};
    char *env[] = {"MY_ENV_VAR=12345", NULL};

    // execl - list arguments with full path
    if ((pid = fork()) == 0) {
        printf("\nUsing execl:\n");
        execl("/bin/ls", "ls", "-l", NULL);
        perror("execl failed");
        exit(1);
    }
    waitpid(pid, &status, 0);

    // execlp - search PATH, list arguments
    if ((pid = fork()) == 0) {
        printf("\nUsing execlp:\n");
        execlp("ls", "ls", "-l", NULL);
        perror("execlp failed");
        exit(1);
    }
    waitpid(pid, &status, 0);

    // execv - array arguments with full path
    if ((pid = fork()) == 0) {
        printf("\nUsing execv:\n");
        execv("/bin/ls", args);
        perror("execv failed");
        exit(1);
    }
    waitpid(pid, &status, 0);

    // execvp - search PATH, array arguments
    if ((pid = fork()) == 0) {
        printf("\nUsing execvp:\n");
        execvp("ls", args);
        perror("execvp failed");
        exit(1);
    }
    waitpid(pid, &status, 0);

    // execle - custom environment, list arguments
    if ((pid = fork()) == 0) {
        printf("\nUsing execle:\n");
        execle("/bin/ls", "ls", "-l", NULL, env);
        perror("execle failed");
        exit(1);
    }
    waitpid(pid, &status, 0);

    // execve - custom environment, array arguments
    if ((pid = fork()) == 0) {
        printf("\nUsing execve:\n");
        execve("/bin/ls", args, env);
        perror("execve failed");
        exit(1);
    }
    waitpid(pid, &status, 0);

    return 0;
}