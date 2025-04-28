#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;
/*
@b
     The "using namespace std;" directive allows us to use names from the 
     standard namespace (like cout, cin, endl) without explicitly prefixing 
      them with "std::". For example, instead of writing "std::cout", we can 
     simply write "cout".
     Advantages:
     1. Simplifies code readability by avoiding repetitive "std::" prefixes.
     2. Useful for small programs or when working extensively with standard library components.

     Disadvantages:
     1. Can lead to name conflicts if other namespaces have similar names.
     2. Not recommended for large projects or header files as it can cause ambiguity.
 */

int main() {
    int a = 10, b = 5;
    for (int i = 0; i < 5; i++) {
        pid_t pid = fork();
        if (pid == 0) { // Child process
            switch(i) {
                case 0: cout << "Child 0: " << a + b << endl; break;
                case 1: cout << "Child 1: " << a - b << endl; break;
                case 2: cout << "Child 2: " << a * b << endl; break;
                case 3: cout << "Child 3: " << a / b << endl; break;
                case 4: cout << "Child 4: " << a % b << endl; break;
            }
            _exit(0); // Terminate child
        } else if (pid < 0) {
            cerr << "Fork failed!" << endl;
            return 1;
        }
    }
    // Parent waits for children
    while (wait(nullptr) > 0);
    return 0;
}