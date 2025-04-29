#!/bin/bash

# --- START of the script ---

# Prompt the user to input the filename they want to search
# -p option lets you add a prompt message directly
read -p "Enter Filename: " fname

# Check if the file exists in the current directory
# [ -e "$fname" ] means "test if a file (any type) exists with the given name"
if [ -e "$fname" ]; then
    # If file exists in the current folder
    echo "File exists in current folder"
else 
    # If file does not exist in current folder
    echo "Searching entire system..."

    # Search the entire filesystem for the file using 'find'
    # find / -name "$fname" => looks for files with exact name under root (/)
    # 2>/dev/null => suppresses permission denied errors
    # head -1 => only take the first found result (if multiple are found)
    found=$(find / -name "$fname" 2>/dev/null | head -1)

    # Now, check if 'found' variable is non-empty (i.e., file was found somewhere)
    if [ "$found" ]; then
        # If file was found somewhere in the system
        echo "Found at: $found"
    else
        # If file was not found anywhere
        echo "File not found"
    fi
fi

# --- END of the script ---
