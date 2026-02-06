#!/bin/bash
# Exercise 4: Functions
#
# Learn to define and use shell functions.
#
# Run: ./04_functions_exercise.sh

# TODO 1: Write a function 'greet' that prints "Hello, <name>!"
# where <name> is the first argument ($1)
greet() {
    ___
}

# TODO 2: Write a function 'add' that adds two numbers
# and outputs (echo) the result
# Hint: Use local variable and arithmetic: $((a + b))
add() {
    local a=$1
    local b=$2
    ___
}

# TODO 3: Write a function 'file_exists' that returns 0 if file exists,
# 1 otherwise. Use 'return' not 'echo'.
file_exists() {
    if [ ___ ]; then
        return ___
    else
        return ___
    fi
}

# TODO 4: Write a function 'log_info' that prints "[INFO] <message>"
# It should accept multiple arguments and print them all
# Hint: Use $* or $@ to get all arguments
log_info() {
    ___
}

# TODO 5: Write a function 'die' that prints an error message to stderr
# and exits with code 1
# Hint: Use >&2 to redirect to stderr
die() {
    ___
    ___
}

# ========== Tests ==========
echo "=== Testing functions ==="

echo -n "Test greet: "
greet "World"

echo -n "Test add: "
result=$(add 5 3)
echo "5 + 3 = $result"

echo -n "Test file_exists: "
if file_exists "/etc/passwd"; then
    echo "/etc/passwd exists"
else
    echo "/etc/passwd not found"
fi

echo -n "Test log_info: "
log_info "Build" "started"

echo -e "\n✓ Functions exercise complete!"
# Uncomment to test die:
# die "This is a test error"
