package main

import (
	"fmt"
	"os"
	"os/exec"
)

func main() {
	cmd := exec.Command("xdg-open", "https://www.google.com")
	err := cmd.Start()
	if err != nil {
		fmt.Println("Error starting command:", err)
		os.Exit(1)
	}
	fmt.Println("Command started successfully, PID:", cmd.Process.Pid)
	cmd.Wait()
	fmt.Println("Command finished successfully")
	if err := cmd.Wait(); err != nil {
		fmt.Println("Error waiting for command:", err)
		os.Exit(1)
	}
	fmt.Println("Command finished successfully")
	fmt.Println("Command output:", cmd.Stdout)
	fmt.Println("Command error output:", cmd.Stderr)
	fmt.Println("Command exit status:", cmd.ProcessState.ExitCode())
	fmt.Println("Command process ID:", cmd.Process.Pid)
	fmt.Println("Command process state:", cmd.ProcessState.String())
}
