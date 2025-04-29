#!/bin/sh
FIFO="myfifo"

# Create FIFO
mkfifo "$FIFO"

# Write to FIFO in background
echo "IPC via FIFO" > "$FIFO" &

# Read from FIFO
cat "$FIFO"

# Cleanup
rm "$FIFO"