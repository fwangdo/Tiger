#!/bin/bash
# Exercise 2: Conditionals and File Tests
#
# Learn how to use if/else statements and test operators.
#
# Run: ./02_conditionals_exercise.sh <filename>
# Example: ./02_conditionals_exercise.sh /etc/passwd

set -u  # Error on undefined variables

FILE="${1:-}"

# TODO 1: Check if FILE variable is empty, print usage and exit with code 1
# Hint: Use -z to test for empty string
if [ ___ ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

# TODO 2: Check if the file exists
# Hint: Use -e for exists, -f for regular file
if [ ___ ]; then
    echo "✓ File exists: $FILE"

    # TODO 3: Check if file is readable
    # Hint: Use -r
    if [ ___ ]; then
        echo "✓ File is readable"
    else
        echo "✗ File is not readable"
    fi

    # TODO 4: Check if file is NOT empty (has size > 0)
    # Hint: Use -s
    if [ ___ ]; then
        echo "✓ File has content"
    else
        echo "✗ File is empty"
    fi

    # TODO 5: Check if file is a directory
    # Hint: Use -d
    if [ ___ ]; then
        echo "→ It's a directory"
    else
        echo "→ It's a regular file"
    fi
else
    echo "✗ File does not exist: $FILE"
    exit 1
fi

echo -e "\nFile check complete!"
