#!/bin/bash

echo "Partitions and Sizes:"
lsblk -o NAME,SIZE,TYPE,MOUNTPOINT

echo -e "\nDisk Usage:"
df -h --output=source,fstype,size,used,avail,pcent

# ## 24. Disk Information
# - `lsblk` shows block devices
#   - `-o`: Custom output columns
# - `df` displays disk usage
#   - `--output`: Select specific fields
#   - `-h`: Human-readable format