#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main() {
    char username[50];
    char password[50];
    pid_t pid = vfork();

    if (pid == 0) { // Child
        cout << "Enter username: ";
        cin >> username;
        _exit(0);
    } else if (pid > 0) { // Parent
        wait(nullptr);
        cout << "Enter password: ";
        cin >> password;
        
        if (strcmp(password, "secret") == 0) {
            cout << "Login successful for user: " << username << endl;
        } else {
            cout << "Invalid password!" << endl;
        }
    } else {
        cerr << "vfork failed!" << endl;
        return 1;
    }
    return 0;
}