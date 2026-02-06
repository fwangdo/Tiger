#!/bin/bash
# Exercise 3: Loops - SOLUTION

echo "=== Part 1: For loop over list ==="
# TODO 1: Loop over the fruits
for fruit in apple banana cherry; do
    echo "Fruit: $fruit"
done

echo -e "\n=== Part 2: C-style for loop ==="
# TODO 2: Loop from 0 to 4
for ((i=0; i<5; i++)); do
    echo "Index: $i"
done

echo -e "\n=== Part 3: Loop over files ==="
# TODO 3: Loop over all .sh files
for file in *.sh; do
    echo "Script: $file"
done

echo -e "\n=== Part 4: While loop ==="
# TODO 4 & 5: While loop with increment
count=0
while [ $count -lt 3 ]; do
    echo "Count: $count"
    ((count++))
done

echo -e "\n=== Part 5: Loop with break ==="
# TODO 6: Loop with break condition
for ((i=1; i<=10; i++)); do
    if [ $i -eq 5 ]; then
        echo "Breaking at $i"
        break
    fi
    echo "Number: $i"
done

echo -e "\nLoops exercise complete!"
