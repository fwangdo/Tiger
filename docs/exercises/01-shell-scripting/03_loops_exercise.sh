#!/bin/bash
# Exercise 3: Loops
#
# Learn for loops, while loops, and iteration patterns.
#
# Run: ./03_loops_exercise.sh

echo "=== Part 1: For loop over list ==="
# TODO 1: Loop over the fruits: apple, banana, cherry
# Print each fruit on a new line
for fruit in apple banana cherry; do
    echo "Fruit: $fruit"
done

echo -e "\n=== Part 2: C-style for loop ==="
# TODO 2: Loop from 0 to 4 using C-style syntax
# Hint: for ((i=start; i<end; i++))
for ((i=0; i < 5; i++)); do
    echo "Index: $i"
done

echo -e "\n=== Part 3: Loop over files ==="
# TODO 3: Loop over all .sh files in current directory
# Hint: Use glob pattern *.sh
for file in *.sh; do
    echo "Script: $file"
done

echo -e "\n=== Part 4: While loop ==="
# TODO 4: While loop - count from 0 while count < 3
count=0
while [ $count -lt 3]; do
    echo "Count: $count"
    # TODO 5: Increment count
    # Hint: Use ((count++)) or count=$((count + 1))
    ((count++))
done

echo -e "\n=== Part 5: Loop with break ==="
# TODO 6: Loop from 1 to 10, but break when i equals 5
for ((i=1; i<=10; i++)); do
    if [ $i -eq 5 ]; then
        echo "Breaking at $i"
        break
    fi
    echo "Number: $i"
done

echo -e "\nLoops exercise complete!"
