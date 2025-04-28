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
        cout << "Child exiting with code 42" << endl;
        _exit(42); // Use _exit instead of exit()
    } else { // Parent
        cout << "Parent (PID: " << getpid() << ") waiting" << endl;
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            cout << "Child exited with code: " << WEXITSTATUS(status) << endl;
        }
    }
    return 0;
}