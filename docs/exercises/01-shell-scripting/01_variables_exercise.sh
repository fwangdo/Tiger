#!/bin/bash
# Exercise 1: Variables and Command Substitution
#
# Learn how to use variables, command substitution, and default values.
#
# Run: ./01_variables_exercise.sh
# Expected output:
#   Greeting: Hello
#   Build dir: build
#   Message: Hello from build
#   Today: 2024-01-15 (current date)
#   Build type: Release

# TODO 1: Assign "Hello" to a variable named GREETING
# Hint: No spaces around the = sign
GREETING=___

# TODO 2: Create a variable BUILD_DIR with value "build"
BUILD_DIR=___

# TODO 3: Print "Hello from build" using both variables
# Hint: Use $VAR or ${VAR} syntax
echo "Message: ___"

# TODO 4: Capture the output of 'date +%Y-%m-%d' into variable TODAY
# Hint: Use $(command) syntax
TODAY=___

# TODO 5: Set BUILD_TYPE with a default value
# If BUILD_TYPE is not set, it should default to "Release"
# Hint: Use ${VAR:-default} syntax
BUILD_TYPE=${___}

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
