#!/bin/bash

# Hide cursor
tput civis
# Show cursor on exit
trap 'tput cnorm; exit' SIGINT

while true; do
    # Print current time in HH:MM:SS format
    printf "\r%s" "$(date +%T)"
    sleep 1
done