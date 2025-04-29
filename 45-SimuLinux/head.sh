#!/bin/bash

# Default number of lines to display
lines=10

# Check if arguments are provided
if [ "$#" -lt 1 ]; then
    echo "Usage: $0 [-n <lines>] <filename>"
    exit 1
fi

# Parse optional `-n` flag
if [ "$1" = "-n" ]; then
    if [ "$#" -lt 3 ]; then
        echo "Error: Missing arguments for -n flag."
        exit 1
    fi
    lines="$2"
    shift 2
fi

file="$1"

# Check if file exists
if [ ! -f "$file" ]; then
    echo "Error: File '$file' does not exist."
    exit 1
fi

# Display the first N lines
head -n "$lines" "$file"