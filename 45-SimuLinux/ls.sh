#!/bin/bash

# Default directory is current directory
dir="."

# If an argument is provided, use it as the directory
if [ "$#" -ge 1 ]; then
    dir="$1"
fi

# List files and directories
if [ -d "$dir" ]; then
    ls -l "$dir"
else
    echo "Error: Directory '$dir' does not exist."
fi