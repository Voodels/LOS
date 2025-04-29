#!/bin/bash

read -p "Enter filename to search: " fname

echo "Searching using locate..."
locate -b -e -l 1 "\/$fname"

echo -e "\nSearching using find..."
find / -name "$fname" 2>/dev/null | head -n 5

# ## 25. File Searching
# - Uses both `locate` and `find`
# - `locate` options:
#   - `-b`: Match basename
#   - `-e`: Check existence
#   - `-l 1`: Limit results
# - `find` searches entire filesystem
# - Limits output with `head`
