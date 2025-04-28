package main

import (
	"os"
	"os/exec"
)

func main() {
	// execvp-style (PATH search)
	cmd1 := exec.Command("ls", "-l")
	cmd1.Stdout = os.Stdout
	cmd1.Run()

	// execv-style (absolute path)
	cmd2 := exec.Command("/bin/ls", "-l")
	cmd2.Stdout = os.Stdout
	cmd2.Run()

	// execve-style (custom environment)
	cmd3 := exec.Command("ls", "-l")
	cmd3.Stdout = os.Stdout
	cmd3.Env = append(os.Environ(), "MY_ENV=test")
	cmd3.Run()
}
