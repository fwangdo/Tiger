#!/bin/bash
# Exercise 1: Variables and Command Substitution - SOLUTION

# TODO 1: Assign "Hello" to a variable named GREETING
GREETING="Hello"

# TODO 2: Create a variable BUILD_DIR with value "build"
BUILD_DIR="build"

# TODO 3: Print "Hello from build" using both variables
echo "Message: $GREETING from $BUILD_DIR"

# TODO 4: Capture the output of 'date +%Y-%m-%d' into variable TODAY
TODAY=$(date +%Y-%m-%d)

# TODO 5: Set BUILD_TYPE with a default value
BUILD_TYPE=${BUILD_TYPE:-Release}

# Print results
echo "Greeting: $GREETING"
echo "Build dir: $BUILD_DIR"
echo "Today: $TODAY"
echo "Build type: $BUILD_TYPE"

# Self-test
if [[ "$GREETING" == "Hello" && "$BUILD_DIR" == "build" ]]; then
    echo -e "\n✓ Exercise completed successfully!"
else
    echo -e "\n✗ Check your answers"
fi
