package main

import (
	"fmt"
	"os"
	"os/exec"
	"syscall"
)

/**
 * - package main:
 *   The starting point of the Go application. Every Go program must have a `main` package
 *   if it needs to be executed.
 *
 * - import:
 *   Importing necessary packages:
 *   - fmt: for formatted I/O operations (like printing).
 *   - os: for accessing environment variables, arguments, and exit functions.
 *   - os/exec: to run external commands or the same program as a new process.
 *   - strconv: for string to integer conversions and vice-versa.
 *   - syscall: for low-level OS functionalities like getting process IDs.
 */

func main() {
	/**
	 * - func main():
	 *   The main function where execution begins.
	 *   Similar to `int main()` in C.
	 */

	// Initialize two integers
	a := 10
	b := 37

	// Check if this process is a child or parent
	args := os.Args
	/**
	 * - os.Args:
	 *   An array containing the command-line arguments.
	 *   os.Args[0] is the program name itself.
	 *   We use additional arguments to differentiate parent and child processes.
	 */

	if len(args) > 1 && args[1] == "child" {
		// Child process logic
		pid := syscall.Getpid()
		/**
		 * - syscall.Getpid():
		 *   Returns the process ID (PID) of the calling process.
		 *   Similar to `getpid()` in C.
		 */

		fmt.Printf("Child process %d: a=%d, b=%d\n", pid, a, b)

		a += 5
		b += 5

		fmt.Printf("Child process %d after modification: a=%d, b=%d\n", pid, a, b)
		os.Exit(0) // Exit child process
		/**
		 * - os.Exit(0):
		 *   Terminates the current program immediately with exit code 0 (success).
		 *   Similar to `_exit(0)` in C.
		 */
	}

	// Parent process logic
	fmt.Printf("Parent process PID=%d\n", os.Getpid())
	for i := 0; i < 5; i++ {
		cmd := exec.Command(os.Args[0], "child")
		/**
		 * - exec.Command():
		 *   Creates a new external command to run.
		 *   Here, we are running the same program (os.Args[0]) with argument "child",
		 *   which triggers the child process logic.
		 */

		cmd.Stdout = os.Stdout
		cmd.Stderr = os.Stderr
		/**
		 * - cmd.Stdout and cmd.Stderr:
		 *   Redirecting the output and errors of the child process to the console,
		 *   so we can see child process output.
		 */

		err := cmd.Start()
		/**
		 * - cmd.Start():
		 *   Starts the specified command but does not wait for it to complete.
		 *   It runs asynchronously (in parallel).
		 */

		if err != nil {
			fmt.Println("Error starting child process:", err)
			continue
		}

		fmt.Printf("Started child process PID=%d\n", cmd.Process.Pid)

		// Wait for the child to complete
		err = cmd.Wait()
		/**
		 * - cmd.Wait():
		 *   Waits for the command to exit.
		 *   Similar to `wait(NULL)` in C.
		 */

		if err != nil {
			fmt.Println("Error waiting for child process:", err)
		}
	}

	fmt.Println("All child processes have terminated.")
}

/*
[Parent Process (main)]
|
|-- Start child 1 --> Child process prints, modifies a, b, exits
|-- Wait for child 1 to finish
|
|-- Start child 2 --> Child process prints, modifies a, b, exits
|-- Wait for child 2 to finish
|
|-- Start child 3 --> ...
|-- Start child 4 --> ...
|-- Start child 5 --> ...
|
|-- Finally, Parent prints "All child processes have terminated."

*/
