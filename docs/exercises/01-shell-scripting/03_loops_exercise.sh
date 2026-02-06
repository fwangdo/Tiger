#!/bin/bash
# Exercise 3: Loops
#
# Learn for loops, while loops, and iteration patterns.
#
# Run: ./03_loops_exercise.sh

echo "=== Part 1: For loop over list ==="
# TODO 1: Loop over the fruits: apple, banana, cherry
# Print each fruit on a new line
for fruit in ___; do
    echo "Fruit: $fruit"
done

echo -e "\n=== Part 2: C-style for loop ==="
# TODO 2: Loop from 0 to 4 using C-style syntax
# Hint: for ((i=start; i<end; i++))
for ((___)); do
    echo "Index: $i"
done

echo -e "\n=== Part 3: Loop over files ==="
# TODO 3: Loop over all .sh files in current directory
# Hint: Use glob pattern *.sh
for file in ___; do
    echo "Script: $file"
done

echo -e "\n=== Part 4: While loop ==="
# TODO 4: While loop - count from 0 while count < 3
count=0
while [ ___ ]; do
    echo "Count: $count"
    # TODO 5: Increment count
    # Hint: Use ((count++)) or count=$((count + 1))
    ___
done

echo -e "\n=== Part 5: Loop with break ==="
# TODO 6: Loop from 1 to 10, but break when i equals 5
for ((i=1; i<=10; i++)); do
    if [ ___ ]; then
        echo "Breaking at $i"
        ___
    fi
    echo "Number: $i"
done

echo -e "\nLoops exercise complete!"
