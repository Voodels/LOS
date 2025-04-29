#!/bin/bash
# ---------------------------------------------
# Bash script to display:
# 1. Currently logged-in users
# 2. All system users
# ---------------------------------------------

# --- Step 1: Show Currently Logged-in Users ---

# echo => Display a message
echo "Logged-in users:"

# who => Lists users currently logged into the system
# output format: username terminal time
# Example line: user1 pts/0 2025-04-29 10:00

# awk '{print $1}' =>
#   For each line, print only the first column (the username)

# sort -u =>
#   sort : Sorts the output alphabetically
#   -u    : Removes duplicate usernames (in case a user has multiple sessions open)

who | awk '{print $1}' | sort -u


# --- Step 2: List All System Users ---

# echo -e => Enable interpretation of backslash escapes
# \n => Print a newline (move to next line before displaying "All users:")

echo -e "\nAll users:"

# getent passwd =>
#   getent : Command to get entries from administrative databases (like /etc/passwd)
#   passwd : Refers specifically to the user database

# Each line from 'getent passwd' looks like:
# username:x:UID:GID:Full Name:Home Directory:Shell

# cut -d: -f1 =>
#   cut : Used to cut sections from input lines
#   -d: : Use colon ':' as the delimiter
#   -f1 : Pick the first field (username)

getent passwd | cut -d: -f1

