
# **2. Network Check Script (network_check.sh)**
```bash
#!/bin/bash

# Test connection to Google DNS
if ping -c 4 8.8.8.8 &> /dev/null; then
    echo "System is connected to the network"
else
    echo "System is not connected to the network"
fi