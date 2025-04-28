package main

import (
	"fmt"
	"os/exec"
	/*
		"os/exec"
		   here it mean exec package from os
	*/)

func main() {
	cmd := exec.Command("sleep", "2")
	fmt.Println("Parent waiting for child...")
	err := cmd.Run() // Run() includes Start() + Wait()
	/*
		// Start() starts the command but does not wait for it to finish
		// Wait() waits for the command to finish
		// Run() is a convenience function that combines Start() and Wait()

	*/
	fmt.Println("Parent finished waiting for child")

	if err != nil {
		fmt.Println("Child exited with error:", err)
	} else {
		fmt.Println("Child exited successfully")
	}
}
