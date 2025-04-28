#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        cerr << "fork failed!" << endl;
        return 1;
    } else if (pid == 0) { // Child
        cout << "Child (PID: " << getpid() << ") started" << endl;
        sleep(2);
        cout << "Child (PID: " << getpid() << ") exiting" << endl;
        _exit(42); // Use _exit to avoid flushing parent's streams
    } else { // Parent
        cout << "Parent (PID: " << getpid() << ") waiting for child..." << endl;
        int status;
        waitpid(pid, &status, 0); // Wait for specific child
        if (WIFEXITED(status)) {
            cout << "Child exited with code: " << WEXITSTATUS(status) << endl;
        }
    }
    return 0;
}