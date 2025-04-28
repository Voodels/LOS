#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main() {
    char username[50];
    char password[50];
    pid_t pid = vfork();

    if (pid == 0) { // Child
        printf("Enter username: ");
        fflush(stdout); // Ensure prompt is shown
        scanf("%s", username);
        _exit(0); // Exit child without flushing buffers
    } else if (pid > 0) { // Parent
        wait(NULL); // Wait for child to finish
        printf("Enter password: ");
        fflush(stdout);
        scanf("%s", password);
        
        // Example validation (password = "secret")
        if (strcmp(password, "secret") == 0) {
            printf("Login successful for user: %s\n", username);
        } else {
            printf("Invalid password!\n");
        }
    } else {
        perror("vfork failed");
        return 1;
    }
    return 0;
}