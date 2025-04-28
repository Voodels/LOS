#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

int main() {
    // execlp (PATH search + list args)
    if (fork() == 0) {
        std::cout << "\nUsing execlp:" << std::endl;
        execlp("ls", "ls", "-l", nullptr);
        std::cerr << "execlp failed!" << std::endl;
        _exit(1);
    }
    wait(nullptr);

    // execv (full path + array args)
    if (fork() == 0) {
        std::cout << "\nUsing execv:" << std::endl;
        const char *args[] = {"ls", "-l", nullptr};
        execv("/bin/ls", const_cast<char**>(args));
        std::cerr << "execv failed!" << std::endl;
        _exit(1);
    }
    wait(nullptr);

    // execve (custom environment)
    if (fork() == 0) {
        std::cout << "\nUsing execve:" << std::endl;
        const char *args[] = {"ls", "-l", nullptr};
        const char *env[] = {"MY_ENV=test", nullptr};
        execve("/bin/ls", const_cast<char**>(args), const_cast<char**>(env));
        std::cerr << "execve failed!" << std::endl;
        _exit(1);
    }
    wait(nullptr);

    return 0;
}