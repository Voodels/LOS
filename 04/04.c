#include<stdio.h>
#include<unistd.h>
/*
    unistd.h
    The unistd.h header file provides access to the POSIX operating system API.
    It includes declarations for various functions and constants related to process control, file I/O, and other system-level operations.
    Some commonly used functions declared in unistd.h include fork(), exec(), read(), write(), and close().
    It also defines constants such as STDIN_FILENO, STDOUT_FILENO, and STDERR_FILENO for standard input, output, and error file descriptors.
*/
#include<sys/wait.h> 
/*
    sys/wait.h
    The sys/wait.h header file provides macros related to process termination.
    It is used in conjunction with the wait() and waitpid() functions to handle process termination.
    The wait() function suspends the calling process until one of its child processes terminates.
    The waitpid() function allows the calling process to wait for a specific child process to terminate.
    It can also be used to wait for any child process if the pid argument is set to -1.
    The macros defined in sys/wait.h are used to interpret the status information returned by these functions.
    These macros include WIFEXITED, WEXITSTATUS, WIFSIGNALED, and WTERMSIG.
*/
int main(){
    pid_t  pid = vfork();
    /*
        fork() creates a new process by duplicating the calling process.
        The new process is referred to as the child process.
        The child process receives a unique process ID (PID) and a copy of the parent's memory space.
        The return value of fork() is 0 in the child process and the child's PID in the parent process.
        If fork() fails, it returns -1 in the parent process.
    */
    if(pid == 0){
        // Child Process
        printf("Child: Opening Calculator...\n");
        execlp("gnome-calculator", "gnome-calculator", NULL);
        // If execlp fails
        perror("execlp");
    }
    else if(pid > 0){
        // Parent Process
        wait(NULL);
        printf("Parent: Child finished.\n");
    }
    else{
        // Fork Failed
        perror("fork");
    }
    /*
        execlp() replaces the current process image with a new process image specified by the given command.
        It searches for the command in the directories listed in the PATH environment variable.
        If execlp() is successful, it does not return to the calling process.
        If it fails, it returns -1 and sets errno to indicate the error.
    */
    /*
        wait() suspends the calling process until one of its child processes terminates.
        It returns the PID of the terminated child process.
        If there are no child processes, it returns -1 and sets errno to indicate the error.
    */
    /*
        perror() prints a description for the last error that occurred.
        It takes a string as an argument, which is printed before the error message.
        The error message is based on the value of errno, which is set by system calls and library functions in case of an error.
    */
    return 0;
}
