package main

import (
	"fmt"
	"os"
	"os/exec"
	"os/signal"
	"syscall"
	"time"
)

func main() {
	if len(os.Args) == 1 { // Parent
		cmd := exec.Command(os.Args[0], "child")
		cmd.Stdout = os.Stdout
		cmd.Start()

		time.Sleep(1 * time.Second) // Allow child setup
		fmt.Printf("Parent: Sending SIGUSR1 to %d\n", cmd.Process.Pid)
		cmd.Process.Signal(syscall.SIGUSR1)
		cmd.Wait()
	} else { // Child
		sig := make(chan os.Signal, 1)
		signal.Notify(sig, syscall.SIGUSR1)

		fmt.Printf("Child: PID %d\n", os.Getpid())
		<-sig // Wait for signal
		fmt.Println("Child: Received SIGUSR1")
		fmt.Println("Child: Exiting")
	}
}
