#!/bin/bash
# -------------------------
# Bash script to download a webpage from a given URL
# using wget command
# -------------------------

# --- Step 1: Ask the user to input a URL ---

# read command is used to take user input
# -p option allows displaying a prompt message before accepting input
# "Enter URL to download: " => Prompt message displayed to user
# url => Variable where the user's input (URL) will be stored
read -p "Enter URL to download: " url

# --- Step 2: Download the webpage using wget ---

# wget : Command-line utility used to download files from the web
# -q : Quiet mode (don't show download progress bar or messages)
# -O webpage.html : Save the downloaded content into a file named 'webpage.html'
# "$url" : The URL provided by the user (quotes are important to handle spaces or special characters safely)

# The '&&' operator:
# - If wget command is successful (exit status 0), then execute 'echo "Download successful"'

# The '||' operator:
# - If wget command fails (exit status not 0), then execute 'echo "Download failed"'

wget -q -O webpage.html "$url" && echo "Download successful" || echo "Download failed"

# --- Additional Explanation ---
# Exit Status:
# - Every command in Linux returns an exit status.
# - 0 => success
# - Non-zero => failure

# && (AND operator):
# - Run the second command only if the first one succeeds.

# || (OR operator):
# - Run the second command only if the first one fails.
