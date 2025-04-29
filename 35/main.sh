#!/bin/bash

# Check if at least 2 arguments are provided
if [ "$#" -lt 2 ]; then
    echo "Error: At least two filename arguments are required."
    echo "Usage: $0 <file1> <file2> [file3...]"
    exit 1
fi

# Function to check if a file exists
fileExists() {
    [ -f "$1" ] # Check if the file exists and is a regular file
}

# Loop through all arguments (filenames)
for filename in "$@"; do
    # Check if the file exists
    if ! fileExists "$filename"; then
        echo "Error: File '$filename' does not exist."
        exit 1
    fi
done

# All files exist, concatenate their contents
echo "Concatenating files..."
cat "$@" # Use the `cat` command to concatenate and display the contents of all files