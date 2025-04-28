#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main() {
    pid_t pid = vfork(); // Child shares parent's memory

    if (pid == 0) { // Child
        execlp("xdg-open", "xdg-open", "https://www.example.com", nullptr);
        _exit(1); // Critical: Use _exit to avoid destroying parent's state
        /*
            _exit(1)
                explain copilot
                    
        */
    } else if (pid > 0) { // Parent
        wait(nullptr);
        cout << "Application opened!" << endl;
    } else {
        cerr << "vfork failed!" << endl;
        return 1;
    }
    return 0;
}