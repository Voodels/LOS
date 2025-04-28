#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        std::cout << "Child: Opening Calculator..." << std::endl;
        execlp("gnome-calculator", "gnome-calculator", NULL);
        perror("execlp");
    } else if (pid > 0) {
        wait(NULL);
        std::cout << "Parent: Child finished." << std::endl;
    } else {
        perror("fork");
    }

    return 0;
}
