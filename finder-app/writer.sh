#!/bin/bash

writefile=$1
writestr=$2

if [ -z "$writefile" ]; then
	echo "writefile arg not provided"
	exit 1
fi

if [ -z "$writestr" ]; then
	echo "writestr arg not provided"
	exit 1
fi

if ! mkdir -p "$(dirname "$writefile")"  2>/dev/null; then
	echo "cannot create directory"
	exit 1
fi

if ! touch "$writefile" 2>/dev/null; then
	echo "cannot create writefile"
	exit 1
fi

echo "$writestr" > "$writefile"

