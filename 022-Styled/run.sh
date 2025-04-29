#!/bin/bash

# --- Defining ANSI Escape Codes for Colors and Effects ---

# RED color code
# \033[ : Escape sequence start
# 1;5;31m : 
#    1 => Bold (bright)
#    5 => Blink (slow blink)
#    31 => Text color Red
RED='\033[1;5;31m'

# BLUE color code
# 1 => Bold
# 5 => Blink
# 34 => Blue color
BLUE='\033[1;5;34m'

# XYZ color code (actually Yellow)
# 1 => Bold
# 5 => Blink
# 33 => Yellow color
XYZ='\033[1;5;33m'

# NC (No Color) - Reset all text formatting
# \033[0m => Reset attributes (normal text)
NC='\033[0m'

# --- Printing Text with Different Colors and then Resetting ---

# -e flag in echo: Enable interpretation of backslash escapes like \033
# Print "Hello World" in RED color and then reset color to normal
echo -e "${RED}Hello World${NC}"

# Print "Hello World" in BLUE color and then reset
echo -e "${BLUE}Hello World${NC}"

# Print "Hello World" in Yellow (named XYZ) color and then reset
echo -e "${XYZ}Hello World
