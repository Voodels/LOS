#!/bin/bash

# Function to check if a file exists
fileExists() {
    [ -e "$1" ]
}

# Check if at least 2 arguments are provided
if [ "$#" -lt 2 ]; then
    echo "Error: At least two filename arguments are required."
    echo "Usage: $0 <file1> <file2> [file3...]"
    exit 1
fi

echo "Processing $# files:"

# Process each file argument
for ((i = 1; i <= $#; i++)); do
    filename="${!i}" # Get the i-th argument
    
    # Check if file exists
    if ! fileExists "$filename"; then
        echo "$i. File '$filename' does not exist."
        continue
    fi

    # Get file information
    if [ -f "$filename" ]; then
        # Regular file
        filesize=$(stat --format="%s" "$filename") # Get file size
        permissions=$(stat --format="%A" "$filename") # Get permissions
        echo "$i. $filename: Regular file, Size: $filesize bytes, Permissions: $permissions"
    elif [ -d "$filename" ]; then
        # Directory
        echo "$i. $filename: Directory"
    else
        # Special file
        echo "$i. $filename: Special file"
    fi
done