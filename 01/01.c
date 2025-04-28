#include<stdio.h>
/**
 * - <stdio.h>:
 *   The standard input/output library in C. It provides functionalities for 
 *   performing input and output operations such as reading from the standard 
 *   input (keyboard) and writing to the standard output (console). Commonly 
 *   used functions from this library include `printf`, `scanf`, `getchar`, 
 *   `putchar`, and file handling functions like `fopen`, `fclose`, etc.
 */
#include<unistd.h>
/**
 * - <unistd.h>:
 *   This header file provides access to the POSIX operating system API. It 
 *   includes declarations for various functions and constants related to 
 *   process control, file operations, and other system-level functionalities. 
 *   Commonly used functions include `fork`, `exec`, `getpid`, `getppid`, 
 *   `read`, `write`, and many others.
 */
#include <sys/wait.h>
/**
 * - <sys/wait.h>:
 *   This header file provides macros related to process termination and 
 *   waiting for child processes. It includes declarations for the `wait` 
 *   function, which allows a parent process to wait for its child processes 
 *   to terminate. It also defines macros like `WIFEXITED`, `WEXITSTATUS`, 
 *   and others that help in checking the termination status of child 
 *   processes.
 */

int main(){
    int a=10,b=37;
    for(int i=0;i<5;i++){
        pid_t pid=fork(); // pid_t is a data type used to represent process IDs
        /**
         * - `fork()`:
         *   This function is used to create a new process by duplicating the 
         *   calling process. The new process created is called the child 
         *   process. The `fork()` function returns a value of 0 to the child 
         *   process and returns the child's PID (process ID) to the parent 
         *   process. If an error occurs, it returns -1.
            *   The child process is an exact copy of the parent process, except
            *  for the returned value of `fork()`. Both processes will execute
            *  the code following the `fork()` call.
            *  The child process has its own memory space, so changes made to
            *  variables in one process do not affect the other.
            *  The child process can execute a different program using `exec()`
            *  functions after the `fork()` call, but in this code, it continues
            *  executing the same code as the parent process.
            *  The `fork()` function is commonly used in Unix-like operating
            *  systems for process creation and management.
            /**
             * @brief Fork()  
             *        -> 0 : Returned to the child process, indicating it is the child.
             *        -> -1 : Indicates an error occurred, and the fork failed.
             *        -> PID : Returned to the parent process, which is the process ID of the child.
             */

        if(pid == -1){
            printf("Fork failed\n");
        }
        else if(pid == 0){
            printf("Child process %d: a=%d, b=%d\n", getpid(), a, b);
            a += 5;
            b += 5;
            printf("Child process %d after modification: a=%d, b=%d\n", getpid(), a, b);
            _exit(0); // Terminate the child process
        }
        else {
            printf("Parent process %d: a=%d, b=%d\n", getpid(), a, b);
            a += (i+1)*5; // modification based on iteration
            b += (i+1)*5;
            printf("Parent process %d after modification: a=%d, b=%d\n", getpid(), a, b);
            wait(NULL); // Wait for child to finish before continuing loop
        }
    }

    /**
     * - `wait(NULL)`:
     *   This function is used by a parent process to wait for its child 
     *   processes to terminate. The `NULL` argument indicates that the parent 
     *   process does not need to retrieve the exit status of the child. The 
     *   function returns the process ID of the terminated child process, or 
     *   -1 if there are no child processes left.
     */
    while (wait(NULL) > 0)
    {
    }
    printf("All child processes have terminated.\n");
    return 0;
}
/**
 * @brief 
 * 1. The program creates multiple child processes using fork().
 * 2. Each child process prints its PID and the values of a and b.
 * 3. The parent process modifies the values of a and b based on the iteration.
 * 4. The parent waits for each child to finish before continuing the loop.
 * 5. Finally, it prints a message indicating that all child processes have terminated.
 */