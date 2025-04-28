package main

import (
	"fmt"
	"os"
	"os/exec"
	"syscall"
	"time"
)

func handleSignal(c chan os.Signal) {
	signal := <-c
	fmt.Printf("Child Process: Received signal %v\n", signal)
}

func main() {
	c := make(chan os.Signal, 1)

	// Create the child process using os/exec
	cmd := exec.Command(os.Args[0]) // Run the same program again to simulate a child process

	// Set the signal handler for the child
	go handleSignal(c)

	// Start the child process
	if err := cmd.Start(); err != nil {
		fmt.Println("Error starting child process:", err)
		return
	}

	// Parent process sends SIGUSR1 signal to the child after a delay
	time.Sleep(2 * time.Second) // Allow time for the child to start
	fmt.Println("Parent Process: Sending SIGUSR1 to child.")

	// Send signal to child process
	pid := cmd.Process.Pid
	syscall.Kill(pid, syscall.SIGUSR1) // Sending SIGUSR1 to the child

	// Wait for the child process to handle the signal
	time.Sleep(1 * time.Second)

	// Wait for the child process to finish
	cmd.Wait()
}
