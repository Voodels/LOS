#!/bin/bash

# Print a heading for the program
echo "--------------------------"
echo " Enter 10 numbers to sort "
echo "--------------------------"

# Read 10 numbers from the user
# -a option is used to read inputs into an array named 'numbers'
read -a numbers

# Notes about 'read' options (ONLY the important real ones):
# -a : Read into an array
# -d : Specify a delimiter (instead of newline)
# -p : Prompt before reading input
# -t : Set a timeout for input
# -n : Read a specific number of characters
# -s : Silent mode (do not display input)
# -e : Use readline features (history, editing)
# -r : Raw input (do not treat backslashes specially)

# Incorrect options like -u, -v, -f, -z etc you wrote above do NOT exist for 'read'.

# -------------------------------
# Now sorting the numbers

# Incorrect in your code: you wrote 'sorted = (...)' 
# There should be NO spaces around '=' in bash assignment.

# Correct assignment syntax:
sorted=($(printf "%s\n" "${numbers[@]}" | sort -n))

# Breakdown:
# printf "%s\n" "${numbers[@]}" => prints each number on a new line
# sort -n => sorts numbers numerically (-n flag)
# $(...) => captures the sorted output into an array

# -------------------------------
# Now displaying the sorted numbers

echo "Sorted numbers: ${sorted[@]}"
