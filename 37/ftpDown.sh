#!/bin/bash
HOST="ftp://10.10.13.16"
FILE="$1"

lftp -e "
set ftp:ssl-allow no;
open $HOST;
ls $FILE;
if \$? == 0 {
    get $FILE;
    exit 0
} else {
    echo 'File not found';
    exit 1
}
"