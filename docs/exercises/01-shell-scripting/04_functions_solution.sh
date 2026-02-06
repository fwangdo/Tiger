#!/bin/bash
# Exercise 4: Functions - SOLUTION

# TODO 1: greet function
greet() {
    echo "Hello, $1!"
}

# TODO 2: add function
add() {
    local a=$1
    local b=$2
    echo $((a + b))
}

# TODO 3: file_exists function
file_exists() {
    if [ -e "$1" ]; then
        return 0
    else
        return 1
    fi
}

# TODO 4: log_info function
log_info() {
    echo "[INFO] $*"
}

# TODO 5: die function
die() {
    echo "[ERROR] $*" >&2
    exit 1
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
