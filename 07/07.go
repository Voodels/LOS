package main

import (
	"fmt"
	"os/exec"
)

func main() {
	cmd := exec.Command("sh", "-c", "exit 42")
	err := cmd.Run()
	if exitErr, ok := err.(*exec.ExitError); ok {
		fmt.Printf("Child exited with code: %d\n", exitErr.ExitCode())
	} else if err != nil {
		fmt.Println("Error:", err)
	} else {
		fmt.Println("Child exited with code 0")
	}
}
