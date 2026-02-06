#!/bin/bash
# Exercise 2: Conditionals and File Tests - SOLUTION

set -u

FILE="${1:-}"

# TODO 1: Check if FILE variable is empty
if [ -z "$FILE" ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

# TODO 2: Check if the file exists
if [ -e "$FILE" ]; then
    echo "✓ File exists: $FILE"

    # TODO 3: Check if file is readable
    if [ -r "$FILE" ]; then
        echo "✓ File is readable"
    else
        echo "✗ File is not readable"
    fi

    # TODO 4: Check if file is NOT empty
    if [ -s "$FILE" ]; then
        echo "✓ File has content"
    else
        echo "✗ File is empty"
    fi

    # TODO 5: Check if file is a directory
    if [ -d "$FILE" ]; then
        echo "→ It's a directory"
    else
        echo "→ It's a regular file"
    fi
else
    echo "✗ File does not exist: $FILE"
    exit 1
fi

echo -e "\nFile check complete!"
