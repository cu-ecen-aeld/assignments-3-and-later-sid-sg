#!/bin/bash

filesdir=$1
searchstr=$2

if [ -z "$filesdir" ]; then
	echo "filesdir arg not provided"
	exit 1
fi

if [ -z "$searchstr" ]; then
	echo "searchstr arg not provided"
	exit 1
fi

if ! [ -d "$filesdir" ]; then
	echo "filesdir is not directory"
	exit 1
fi

X=$(grep -rl "$searchstr" "$filesdir" 2>/dev/null | wc -l)

Y=$(grep -r "$searchstr" "$filesdir" 2>/dev/null | wc -l)

echo "The number of files are $X and the number of matching lines are $Y"

