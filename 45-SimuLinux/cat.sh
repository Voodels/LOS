#!/bin/bash

# Check if at least one argument is provided
if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

# Loop through all files and display their contents
for file in "$@"; do
    if [ -f "$file" ]; then
        cat "$file"
    else
        echo "Error: File '$file' does not exist."
    fi
done