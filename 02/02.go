package main

/*
#include <unistd.h>
#include <sys/wait.h>
*/
import "C"

import (
	"fmt"
	"syscall"
)

func main() {
	var username, password string

	// Use vfork via C (Linux-specific)
	pid, _, err := syscall.Syscall(syscall.SYS_VFORK, 0, 0, 0)
	if err != 0 {
		fmt.Println("vfork error:", err)
		return
	}

	if pid == 0 { // Child process
		fmt.Print("Enter username: ")
		fmt.Scan(&username)
		syscall.Exit(0) // Use _exit(0) in C to avoid flushing shared buffers
	} else { // Parent process
		var wstatus C.int
		C.waitpid(C.int(pid), &wstatus, 0) // Wait for child
		fmt.Print("Enter password: ")
		fmt.Scan(&password)
		if password == "secret" {
			fmt.Printf("Login successful for user: %s\n", username)
		} else {
			fmt.Println("Invalid password!")
		}
	}
}
