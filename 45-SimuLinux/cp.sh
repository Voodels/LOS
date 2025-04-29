#!/bin/bash

# Check if two arguments are provided (source and destination)
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <source> <destination>"
    exit 1
fi

source="$1"
destination="$2"

# Check if source file exists
if [ ! -f "$source" ]; then
    echo "Error: Source file '$source' does not exist."
    exit 1
fi

# Copy the file
cp "$source" "$destination"
if [ $? -eq 0 ]; then
    echo "File '$source' copied to '$destination'."
else
    echo "Error: Failed to copy file."
fi