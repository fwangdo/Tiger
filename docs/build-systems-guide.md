# Build Systems & Shell Scripts: A Comprehensive Guide

Learn to automate C++ builds using CMake and shell scripts.

---

## Table of Contents

1. [Part 1: Shell Scripting Fundamentals](#part-1-shell-scripting-fundamentals)
2. [Part 2: CMake Basics](#part-2-cmake-basics)
3. [Part 3: Advanced CMake](#part-3-advanced-cmake)
4. [Part 4: Build Automation Scripts](#part-4-build-automation-scripts)
5. [Part 5: Real-World Build Script Examples](#part-5-real-world-build-script-examples)

---

## Part 1: Shell Scripting Fundamentals

### 1.1 What is a Shell Script?

A shell script is a text file containing commands that the shell executes sequentially. It automates repetitive tasks.

```bash
#!/bin/bash
# This is a comment
echo "Hello, World!"
```

The first line `#!/bin/bash` is called a **shebang** - it tells the system which interpreter to use.

### 1.2 Creating and Running Scripts

```bash
# Create a script
cat > myscript.sh << 'EOF'
#!/bin/bash
echo "Hello from script!"
EOF

# Make it executable
chmod +x myscript.sh

# Run it
./myscript.sh
```

### 1.3 Variables

```bash
#!/bin/bash

# Variable assignment (NO spaces around =)
NAME="Tiger"
VERSION=1.0
BUILD_DIR="build"

# Using variables (use $VAR or ${VAR})
echo "Building $NAME version $VERSION"
echo "Output directory: ${BUILD_DIR}"

# Command substitution - capture command output
CURRENT_DIR=$(pwd)
DATE=$(date +%Y-%m-%d)
FILE_COUNT=$(ls -1 | wc -l)

echo "Current directory: $CURRENT_DIR"
echo "Date: $DATE"
echo "Files in directory: $FILE_COUNT"

# Environment variables
echo "Home directory: $HOME"
echo "Current user: $USER"
echo "Path: $PATH"
```

### 1.4 Special Variables

| Variable | Meaning |
|----------|---------|
| `$0` | Script name |
| `$1, $2, ...` | Positional arguments |
| `$#` | Number of arguments |
| `$@` | All arguments as separate words |
| `$*` | All arguments as single string |
| `$?` | Exit status of last command |
| `$$` | Process ID of current script |
| `$!` | Process ID of last background command |

```bash
#!/bin/bash

echo "Script name: $0"
echo "First argument: $1"
echo "Second argument: $2"
echo "Number of arguments: $#"
echo "All arguments: $@"
```

### 1.5 Conditionals

#### Basic if-else

```bash
#!/bin/bash

if [ condition ]; then
    # commands
elif [ another_condition ]; then
    # commands
else
    # commands
fi
```

#### String Comparisons

```bash
#!/bin/bash

STRING="hello"

# String equality
if [ "$STRING" = "hello" ]; then
    echo "Strings are equal"
fi

# String inequality
if [ "$STRING" != "world" ]; then
    echo "Strings are not equal"
fi

# Empty string check
if [ -z "$STRING" ]; then
    echo "String is empty"
fi

# Non-empty string check
if [ -n "$STRING" ]; then
    echo "String is not empty"
fi
```

#### Numeric Comparisons

```bash
#!/bin/bash

NUM=10

# Use -eq, -ne, -lt, -le, -gt, -ge for numbers
if [ "$NUM" -eq 10 ]; then
    echo "Equal to 10"
fi

if [ "$NUM" -gt 5 ]; then
    echo "Greater than 5"
fi

if [ "$NUM" -le 20 ]; then
    echo "Less than or equal to 20"
fi
```

| Operator | Meaning |
|----------|---------|
| `-eq` | Equal |
| `-ne` | Not equal |
| `-lt` | Less than |
| `-le` | Less than or equal |
| `-gt` | Greater than |
| `-ge` | Greater than or equal |

#### File Tests

```bash
#!/bin/bash

FILE="test.txt"
DIR="build"

# File exists
if [ -e "$FILE" ]; then
    echo "File exists"
fi

# Is a regular file
if [ -f "$FILE" ]; then
    echo "Is a regular file"
fi

# Is a directory
if [ -d "$DIR" ]; then
    echo "Is a directory"
fi

# File is readable/writable/executable
if [ -r "$FILE" ]; then echo "Readable"; fi
if [ -w "$FILE" ]; then echo "Writable"; fi
if [ -x "$FILE" ]; then echo "Executable"; fi

# File is not empty
if [ -s "$FILE" ]; then
    echo "File is not empty"
fi
```

| Operator | Meaning |
|----------|---------|
| `-e` | Exists |
| `-f` | Is regular file |
| `-d` | Is directory |
| `-r` | Is readable |
| `-w` | Is writable |
| `-x` | Is executable |
| `-s` | Is not empty |
| `-L` | Is symbolic link |

#### Logical Operators

```bash
#!/bin/bash

# AND: -a or &&
if [ -f "$FILE" ] && [ -r "$FILE" ]; then
    echo "File exists and is readable"
fi

# OR: -o or ||
if [ -f "$FILE" ] || [ -d "$FILE" ]; then
    echo "Path exists as file or directory"
fi

# NOT: !
if [ ! -e "$FILE" ]; then
    echo "File does not exist"
fi
```

#### Modern Test Syntax [[ ]]

```bash
#!/bin/bash

# [[ ]] is more powerful than [ ]
# - Supports pattern matching
# - Supports regex with =~
# - No word splitting issues

STRING="hello world"

# Pattern matching
if [[ "$STRING" == hello* ]]; then
    echo "Starts with hello"
fi

# Regex matching
if [[ "$STRING" =~ ^hello ]]; then
    echo "Matches regex ^hello"
fi

# Safe variable expansion (no quotes needed)
if [[ $STRING == "hello world" ]]; then
    echo "Equal"
fi
```

### 1.6 Loops

#### For Loop

```bash
#!/bin/bash

# Loop over list
for item in apple banana cherry; do
    echo "Fruit: $item"
done

# Loop over files
for file in *.cpp; do
    echo "C++ file: $file"
done

# Loop over command output
for user in $(cat /etc/passwd | cut -d: -f1); do
    echo "User: $user"
done

# C-style for loop
for ((i = 0; i < 5; i++)); do
    echo "Index: $i"
done

# Loop over arguments
for arg in "$@"; do
    echo "Argument: $arg"
done
```

#### While Loop

```bash
#!/bin/bash

# Counter-based while loop
count=0
while [ $count -lt 5 ]; do
    echo "Count: $count"
    ((count++))
done

# Read file line by line
while IFS= read -r line; do
    echo "Line: $line"
done < input.txt

# Infinite loop with break
while true; do
    echo "Running..."
    if [ some_condition ]; then
        break
    fi
    sleep 1
done
```

#### Until Loop

```bash
#!/bin/bash

# Until loop - runs until condition is true
count=0
until [ $count -ge 5 ]; do
    echo "Count: $count"
    ((count++))
done
```

### 1.7 Functions

```bash
#!/bin/bash

# Function definition
function greet() {
    echo "Hello, $1!"
}

# Alternative syntax
say_goodbye() {
    echo "Goodbye, $1!"
}

# Function with return value
add_numbers() {
    local result=$(( $1 + $2 ))
    echo $result  # "Return" via stdout
}

# Function with exit status
check_file() {
    if [ -f "$1" ]; then
        return 0  # Success
    else
        return 1  # Failure
    fi
}

# Using functions
greet "World"
say_goodbye "World"

sum=$(add_numbers 5 3)
echo "Sum: $sum"

if check_file "test.txt"; then
    echo "File exists"
else
    echo "File not found"
fi
```

#### Local Variables

```bash
#!/bin/bash

GLOBAL_VAR="I'm global"

my_function() {
    local LOCAL_VAR="I'm local"
    GLOBAL_VAR="Modified global"

    echo "Inside function:"
    echo "  Local: $LOCAL_VAR"
    echo "  Global: $GLOBAL_VAR"
}

my_function

echo "Outside function:"
echo "  Local: $LOCAL_VAR"    # Empty - local is not accessible
echo "  Global: $GLOBAL_VAR"  # Modified
```

### 1.8 Arrays

```bash
#!/bin/bash

# Array declaration
FRUITS=("apple" "banana" "cherry")

# Accessing elements (0-indexed)
echo "First fruit: ${FRUITS[0]}"
echo "Second fruit: ${FRUITS[1]}"

# All elements
echo "All fruits: ${FRUITS[@]}"

# Number of elements
echo "Count: ${#FRUITS[@]}"

# Adding elements
FRUITS+=("date")

# Looping over array
for fruit in "${FRUITS[@]}"; do
    echo "Fruit: $fruit"
done

# Associative arrays (bash 4+)
declare -A VERSIONS
VERSIONS["gcc"]="11.0"
VERSIONS["clang"]="14.0"
VERSIONS["cmake"]="3.25"

echo "GCC version: ${VERSIONS[gcc]}"

# Loop over associative array
for key in "${!VERSIONS[@]}"; do
    echo "$key: ${VERSIONS[$key]}"
done
```

### 1.9 Input/Output

#### Reading Input

```bash
#!/bin/bash

# Read from user
echo -n "Enter your name: "
read NAME
echo "Hello, $NAME!"

# Read with prompt
read -p "Enter age: " AGE

# Read silently (for passwords)
read -s -p "Enter password: " PASSWORD
echo  # newline after silent read

# Read with timeout
if read -t 5 -p "Quick! Enter something: " QUICK; then
    echo "You entered: $QUICK"
else
    echo "Too slow!"
fi

# Read with default value
read -p "Build type [Release]: " BUILD_TYPE
BUILD_TYPE=${BUILD_TYPE:-Release}
```

#### Output and Redirection

```bash
#!/bin/bash

# Standard output
echo "Normal message"

# Standard error
echo "Error message" >&2

# Redirect stdout to file
echo "Log entry" > output.log      # Overwrite
echo "Another entry" >> output.log # Append

# Redirect stderr to file
command 2> error.log

# Redirect both stdout and stderr
command > all.log 2>&1
command &> all.log  # Shorthand (bash)

# Discard output
command > /dev/null 2>&1

# Pipe output to another command
cat file.txt | grep "pattern" | sort | uniq
```

### 1.10 Error Handling

```bash
#!/bin/bash

# Exit on error
set -e

# Exit on undefined variable
set -u

# Fail on pipe errors
set -o pipefail

# Combined (recommended for scripts)
set -euo pipefail

# Check command success
if ! command -v cmake &> /dev/null; then
    echo "Error: cmake is not installed" >&2
    exit 1
fi

# Trap errors
trap 'echo "Error on line $LINENO"; exit 1' ERR

# Cleanup on exit
cleanup() {
    echo "Cleaning up..."
    rm -rf "$TEMP_DIR"
}
trap cleanup EXIT

# Custom error function
die() {
    echo "Error: $1" >&2
    exit "${2:-1}"
}

# Usage
[ -f "config.txt" ] || die "Config file not found" 2
```

### 1.11 Useful Patterns

#### Default Values

```bash
#!/bin/bash

# Default value if unset or empty
BUILD_TYPE=${BUILD_TYPE:-Release}

# Default value if unset only
BUILD_TYPE=${BUILD_TYPE-Release}

# Assign default if unset
: ${BUILD_TYPE:=Release}

# Error if unset
: ${REQUIRED_VAR:?"REQUIRED_VAR must be set"}
```

#### String Manipulation

```bash
#!/bin/bash

STRING="Hello, World!"

# Length
echo "Length: ${#STRING}"

# Substring (offset, length)
echo "Substring: ${STRING:0:5}"   # "Hello"
echo "From index 7: ${STRING:7}"  # "World!"

# Replace first occurrence
echo "${STRING/World/Universe}"   # "Hello, Universe!"

# Replace all occurrences
echo "${STRING//o/0}"             # "Hell0, W0rld!"

# Remove prefix pattern
FILE="document.txt"
echo "${FILE%.txt}"               # "document"

# Remove suffix pattern
PATH="/home/user/file.txt"
echo "${PATH##*/}"                # "file.txt" (basename)
echo "${PATH%/*}"                 # "/home/user" (dirname)
```

#### Command-Line Argument Parsing

```bash
#!/bin/bash

# Simple positional arguments
usage() {
    echo "Usage: $0 [-d] [-t type] [-o output] input_file"
    exit 1
}

# Parse options with getopts
DEBUG=false
BUILD_TYPE="Release"
OUTPUT_DIR="build"

while getopts "dt:o:h" opt; do
    case $opt in
        d) DEBUG=true ;;
        t) BUILD_TYPE="$OPTARG" ;;
        o) OUTPUT_DIR="$OPTARG" ;;
        h) usage ;;
        ?) usage ;;
    esac
done

# Shift to get remaining positional arguments
shift $((OPTIND - 1))

if [ $# -lt 1 ]; then
    usage
fi

INPUT_FILE="$1"

echo "Debug: $DEBUG"
echo "Build type: $BUILD_TYPE"
echo "Output: $OUTPUT_DIR"
echo "Input: $INPUT_FILE"
```

---

## Part 2: CMake Basics

### 2.1 What is CMake?

CMake is a **meta-build system** - it generates build files for other build systems (Make, Ninja, Visual Studio, Xcode).

```
┌──────────────┐     ┌─────────────┐     ┌──────────────┐     ┌────────────┐
│ CMakeLists   │────▶│   CMake     │────▶│  Makefile/   │────▶│ Executable │
│    .txt      │     │ (configure) │     │  Ninja/etc   │     │  (build)   │
└──────────────┘     └─────────────┘     └──────────────┘     └────────────┘
```

### 2.2 Basic CMakeLists.txt

```cmake
# Minimum CMake version required
cmake_minimum_required(VERSION 3.16)

# Project name and language
project(MyCompiler VERSION 1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create executable from source files
add_executable(mycompiler
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/token.cpp
)

# Include directories
target_include_directories(mycompiler PRIVATE src)
```

### 2.3 Building with CMake

```bash
# Standard out-of-source build
mkdir build
cd build
cmake ..            # Configure (generate build files)
cmake --build .     # Build

# Or specify generator
cmake -G "Unix Makefiles" ..
cmake -G "Ninja" ..

# Build with specific target
cmake --build . --target mycompiler

# Build with parallel jobs
cmake --build . -j8
cmake --build . --parallel 8
```

### 2.4 CMake Variables

```cmake
# Setting variables
set(MY_VAR "value")
set(MY_LIST "item1" "item2" "item3")

# Using variables
message(STATUS "My var: ${MY_VAR}")

# Cache variables (user-configurable)
set(MY_OPTION "default" CACHE STRING "Description of option")

# Boolean option
option(ENABLE_TESTS "Build tests" ON)

# Environment variables
set(ENV{MY_ENV_VAR} "value")
message(STATUS "Home: $ENV{HOME}")
```

### 2.5 Important Built-in Variables

| Variable | Description |
|----------|-------------|
| `CMAKE_SOURCE_DIR` | Top-level source directory |
| `CMAKE_BINARY_DIR` | Top-level build directory |
| `CMAKE_CURRENT_SOURCE_DIR` | Current CMakeLists.txt directory |
| `CMAKE_CURRENT_BINARY_DIR` | Current build directory |
| `CMAKE_CXX_COMPILER` | C++ compiler path |
| `CMAKE_BUILD_TYPE` | Build type (Debug, Release, etc.) |
| `CMAKE_INSTALL_PREFIX` | Installation prefix |
| `PROJECT_NAME` | Current project name |
| `PROJECT_VERSION` | Current project version |

### 2.6 Build Types

```cmake
# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Available build types:
# - Debug: -g (debug symbols, no optimization)
# - Release: -O3 -DNDEBUG (optimized, no debug)
# - RelWithDebInfo: -O2 -g -DNDEBUG (optimized with debug)
# - MinSizeRel: -Os -DNDEBUG (size optimized)
```

From command line:
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake -DCMAKE_BUILD_TYPE=Release ..
```

### 2.7 Compiler Flags

```cmake
# Add compile options to a target
target_compile_options(mycompiler PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# Conditional flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(mycompiler PRIVATE -Werror)
endif()

# Debug-specific flags
target_compile_options(mycompiler PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address>
)

# Generator expressions for conditional compilation
target_compile_definitions(mycompiler PRIVATE
    $<$<CONFIG:Debug>:DEBUG_MODE>
    $<$<CONFIG:Release>:NDEBUG>
)
```

### 2.8 Finding Source Files

```cmake
# Explicit list (preferred for clarity)
set(SOURCES
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
)

# Glob (use with caution - doesn't detect new files automatically)
file(GLOB SOURCES "src/*.cpp")
file(GLOB_RECURSE SOURCES "src/**/*.cpp")

# Better: CONFIGURE_DEPENDS (CMake 3.12+)
file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(mycompiler ${SOURCES})
```

### 2.9 Subdirectories and Libraries

```cmake
# Project structure:
# project/
# ├── CMakeLists.txt
# ├── src/
# │   ├── CMakeLists.txt
# │   └── *.cpp
# └── lib/
#     ├── CMakeLists.txt
#     └── *.cpp

# Root CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(MyProject)

add_subdirectory(lib)
add_subdirectory(src)

# lib/CMakeLists.txt
add_library(mylib STATIC
    lexer.cpp
    parser.cpp
)
target_include_directories(mylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# src/CMakeLists.txt
add_executable(mycompiler main.cpp)
target_link_libraries(mycompiler PRIVATE mylib)
```

### 2.10 Installing

```cmake
# Install executable
install(TARGETS mycompiler
    RUNTIME DESTINATION bin
)

# Install headers
install(FILES src/mylib.h
    DESTINATION include
)

# Install directory
install(DIRECTORY include/
    DESTINATION include
)
```

```bash
# Configure with install prefix
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..

# Build and install
cmake --build .
cmake --install .
# Or: make install
```

---

## Part 3: Advanced CMake

### 3.1 Conditionals

```cmake
# if-elseif-else-endif
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release build")
else()
    message(STATUS "Other build type")
endif()

# Boolean checks
if(ENABLE_TESTS)
    add_subdirectory(tests)
endif()

# Variable defined check
if(DEFINED MY_VAR)
    message(STATUS "MY_VAR is defined")
endif()

# String checks
if(MY_STRING STREQUAL "expected")
    # ...
endif()

if(MY_STRING MATCHES "^prefix")
    # Regex match
endif()

# Version checks
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.20")
    # Use newer features
endif()

# File checks
if(EXISTS "${CMAKE_SOURCE_DIR}/optional.cpp")
    list(APPEND SOURCES optional.cpp)
endif()
```

### 3.2 Functions and Macros

```cmake
# Function (has its own scope)
function(print_target_info target)
    get_target_property(SOURCES ${target} SOURCES)
    message(STATUS "Target ${target} sources: ${SOURCES}")
endfunction()

# Macro (no scope - like text substitution)
macro(add_my_executable name)
    add_executable(${name} ${ARGN})
    target_compile_options(${name} PRIVATE -Wall)
endmacro()

# Usage
print_target_info(mycompiler)
add_my_executable(myapp main.cpp utils.cpp)
```

### 3.3 Finding Packages

```cmake
# Find installed packages
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost 1.70 COMPONENTS filesystem system REQUIRED)

# Link against found packages
target_link_libraries(myapp PRIVATE
    Threads::Threads
    OpenSSL::SSL
    Boost::filesystem
)

# Optional packages
find_package(Doxygen)
if(Doxygen_FOUND)
    # Add documentation target
endif()
```

### 3.4 Testing with CTest

```cmake
# Enable testing
enable_testing()

# Add test executable
add_executable(test_lexer tests/test_lexer.cpp)
target_link_libraries(test_lexer PRIVATE mylib)

# Register tests
add_test(NAME LexerTest COMMAND test_lexer)
add_test(NAME ParserTest COMMAND test_parser)

# Test with arguments
add_test(NAME IntegrationTest
    COMMAND mycompiler --parse ${CMAKE_SOURCE_DIR}/tests/sample.tig
)

# Set test properties
set_tests_properties(LexerTest PROPERTIES
    TIMEOUT 30
    LABELS "unit"
)
```

```bash
# Run all tests
ctest

# Run with verbose output
ctest -V

# Run specific test
ctest -R LexerTest

# Run tests matching label
ctest -L unit
```

### 3.5 Custom Commands and Targets

```cmake
# Custom command - runs during build
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/generated.cpp
    COMMAND python3 ${CMAKE_SOURCE_DIR}/generate.py > generated.cpp
    DEPENDS ${CMAKE_SOURCE_DIR}/generate.py
    COMMENT "Generating source file..."
)

# Use generated file
add_executable(myapp main.cpp ${CMAKE_BINARY_DIR}/generated.cpp)

# Custom target - can be built explicitly
add_custom_target(format
    COMMAND clang-format -i ${SOURCES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Formatting source files..."
)

# Custom target that always runs
add_custom_target(run
    COMMAND mycompiler ${CMAKE_SOURCE_DIR}/examples/test.tig
    DEPENDS mycompiler
    COMMENT "Running compiler..."
)
```

### 3.6 Generator Expressions

Generator expressions are evaluated at build time, not configure time.

```cmake
# Conditional based on build type
target_compile_definitions(myapp PRIVATE
    $<$<CONFIG:Debug>:DEBUG=1>
    $<$<CONFIG:Release>:NDEBUG>
)

# Conditional based on compiler
target_compile_options(myapp PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-fno-rtti>
    $<$<CXX_COMPILER_ID:Clang>:-fno-rtti>
    $<$<CXX_COMPILER_ID:MSVC>:/GR->
)

# Platform-specific
target_compile_definitions(myapp PRIVATE
    $<$<PLATFORM_ID:Windows>:WINDOWS>
    $<$<PLATFORM_ID:Linux>:LINUX>
    $<$<PLATFORM_ID:Darwin>:MACOS>
)

# Boolean expressions
$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>
$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>
$<NOT:$<CONFIG:Debug>>

# Conditional include directories
target_include_directories(myapp PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
```

### 3.7 Presets (CMake 3.19+)

Create `CMakePresets.json` for reproducible builds:

```json
{
    "version": 3,
    "cmakeMinimumRequired": {
        "major": 3,
        "minor": 19,
        "patch": 0
    },
    "configurePresets": [
        {
            "name": "debug",
            "displayName": "Debug",
            "binaryDir": "${sourceDir}/build/debug",
            "cacheVariables": {
                "CMAKE_BUILD_TYPE": "Debug",
                "ENABLE_TESTS": "ON"
            }
        },
        {
            "name": "release",
            "displayName": "Release",
            "binaryDir": "${sourceDir}/build/release",
            "cacheVariables": {
                "CMAKE_BUILD_TYPE": "Release",
                "ENABLE_TESTS": "OFF"
            }
        },
        {
            "name": "ninja-release",
            "inherits": "release",
            "generator": "Ninja"
        }
    ],
    "buildPresets": [
        {
            "name": "debug",
            "configurePreset": "debug"
        },
        {
            "name": "release",
            "configurePreset": "release"
        }
    ]
}
```

Usage:
```bash
cmake --preset debug
cmake --build --preset debug
```

---

## Part 4: Build Automation Scripts

### 4.1 Basic Build Script

```bash
#!/bin/bash
set -euo pipefail

# Configuration
PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"
BUILD_TYPE="Release"
GENERATOR="Unix Makefiles"
PARALLEL_JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Logging functions
log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1" >&2
}

die() {
    log_error "$1"
    exit "${2:-1}"
}

# Check prerequisites
check_prerequisites() {
    log_info "Checking prerequisites..."

    command -v cmake &>/dev/null || die "cmake is not installed"
    command -v g++ &>/dev/null || command -v clang++ &>/dev/null || die "No C++ compiler found"

    log_info "All prerequisites satisfied"
}

# Configure project
configure() {
    log_info "Configuring project (${BUILD_TYPE})..."

    mkdir -p "${BUILD_DIR}"

    cmake -S "${PROJECT_ROOT}" \
          -B "${BUILD_DIR}" \
          -G "${GENERATOR}" \
          -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
          -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

    log_info "Configuration complete"
}

# Build project
build() {
    log_info "Building project with ${PARALLEL_JOBS} parallel jobs..."

    cmake --build "${BUILD_DIR}" \
          --parallel "${PARALLEL_JOBS}"

    log_info "Build complete"
}

# Clean build
clean() {
    log_info "Cleaning build directory..."
    rm -rf "${BUILD_DIR}"
    log_info "Clean complete"
}

# Run tests
test() {
    log_info "Running tests..."

    cd "${BUILD_DIR}"
    ctest --output-on-failure

    log_info "Tests complete"
}

# Main
main() {
    check_prerequisites
    configure
    build
}

main "$@"
```

### 4.2 Full-Featured Build Script

```bash
#!/bin/bash
#
# build.sh - Build script for the Tiger compiler
#
# Usage: ./build.sh [options] [command]
#
# Commands:
#   configure   Configure the project (default if no command)
#   build       Build the project
#   test        Run tests
#   clean       Clean build directory
#   install     Install the project
#   all         Configure, build, and test
#
# Options:
#   -t, --type TYPE     Build type: Debug, Release, RelWithDebInfo (default: Release)
#   -g, --generator GEN CMake generator: make, ninja (default: auto-detect)
#   -j, --jobs N        Number of parallel jobs (default: auto-detect)
#   -p, --prefix PATH   Installation prefix (default: /usr/local)
#   -c, --clean         Clean before building
#   -v, --verbose       Verbose output
#   -h, --help          Show this help message

set -euo pipefail

#######################################
# Configuration
#######################################

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="${SCRIPT_DIR}"
BUILD_DIR="${PROJECT_ROOT}/build"
BUILD_TYPE="Release"
INSTALL_PREFIX="/usr/local"
VERBOSE=false
CLEAN_FIRST=false

# Auto-detect settings
detect_jobs() {
    if command -v nproc &>/dev/null; then
        nproc
    elif command -v sysctl &>/dev/null; then
        sysctl -n hw.ncpu
    else
        echo 4
    fi
}

detect_generator() {
    if command -v ninja &>/dev/null; then
        echo "Ninja"
    else
        echo "Unix Makefiles"
    fi
}

PARALLEL_JOBS=$(detect_jobs)
GENERATOR=$(detect_generator)

#######################################
# Colors and Logging
#######################################

if [[ -t 1 ]]; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    YELLOW='\033[1;33m'
    BLUE='\033[0;34m'
    BOLD='\033[1m'
    NC='\033[0m'
else
    RED=''
    GREEN=''
    YELLOW=''
    BLUE=''
    BOLD=''
    NC=''
fi

log_info()    { echo -e "${GREEN}[INFO]${NC} $*"; }
log_warn()    { echo -e "${YELLOW}[WARN]${NC} $*"; }
log_error()   { echo -e "${RED}[ERROR]${NC} $*" >&2; }
log_debug()   { [[ "${VERBOSE}" == true ]] && echo -e "${BLUE}[DEBUG]${NC} $*" || true; }
log_section() { echo -e "\n${BOLD}=== $* ===${NC}"; }

die() {
    log_error "$1"
    exit "${2:-1}"
}

#######################################
# Helper Functions
#######################################

check_command() {
    command -v "$1" &>/dev/null
}

ensure_command() {
    check_command "$1" || die "$1 is required but not installed"
}

measure_time() {
    local start_time=$(date +%s)
    "$@"
    local end_time=$(date +%s)
    local duration=$((end_time - start_time))
    log_info "Completed in ${duration}s"
}

#######################################
# Build Functions
#######################################

check_prerequisites() {
    log_section "Checking Prerequisites"

    ensure_command cmake

    if ! check_command g++ && ! check_command clang++; then
        die "No C++ compiler found (need g++ or clang++)"
    fi

    if [[ "${GENERATOR}" == "Ninja" ]]; then
        ensure_command ninja
    fi

    local cmake_version
    cmake_version=$(cmake --version | head -1 | grep -oE '[0-9]+\.[0-9]+')
    log_info "CMake version: ${cmake_version}"

    local cxx_compiler
    cxx_compiler=$(command -v g++ || command -v clang++)
    log_info "C++ compiler: ${cxx_compiler}"

    log_info "Generator: ${GENERATOR}"
    log_info "Parallel jobs: ${PARALLEL_JOBS}"
}

do_configure() {
    log_section "Configuring (${BUILD_TYPE})"

    mkdir -p "${BUILD_DIR}"

    local cmake_args=(
        -S "${PROJECT_ROOT}"
        -B "${BUILD_DIR}"
        -G "${GENERATOR}"
        -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"
        -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}"
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    )

    if [[ "${VERBOSE}" == true ]]; then
        cmake_args+=(-DCMAKE_VERBOSE_MAKEFILE=ON)
    fi

    log_debug "cmake ${cmake_args[*]}"

    measure_time cmake "${cmake_args[@]}"

    # Create symlink to compile_commands.json for IDE support
    if [[ -f "${BUILD_DIR}/compile_commands.json" ]]; then
        ln -sf "${BUILD_DIR}/compile_commands.json" "${PROJECT_ROOT}/compile_commands.json"
        log_info "Created compile_commands.json symlink"
    fi
}

do_build() {
    log_section "Building"

    if [[ ! -f "${BUILD_DIR}/CMakeCache.txt" ]]; then
        log_warn "Project not configured, running configure first..."
        do_configure
    fi

    local build_args=(
        --build "${BUILD_DIR}"
        --parallel "${PARALLEL_JOBS}"
    )

    if [[ "${VERBOSE}" == true ]]; then
        build_args+=(--verbose)
    fi

    log_debug "cmake ${build_args[*]}"

    measure_time cmake "${build_args[@]}"
}

do_test() {
    log_section "Running Tests"

    if [[ ! -f "${BUILD_DIR}/CMakeCache.txt" ]]; then
        die "Project not built. Run: $0 build"
    fi

    cd "${BUILD_DIR}"

    local ctest_args=(
        --output-on-failure
        --parallel "${PARALLEL_JOBS}"
    )

    if [[ "${VERBOSE}" == true ]]; then
        ctest_args+=(--verbose)
    fi

    measure_time ctest "${ctest_args[@]}"
}

do_clean() {
    log_section "Cleaning"

    if [[ -d "${BUILD_DIR}" ]]; then
        rm -rf "${BUILD_DIR}"
        log_info "Removed ${BUILD_DIR}"
    else
        log_info "Build directory already clean"
    fi

    # Remove symlink
    if [[ -L "${PROJECT_ROOT}/compile_commands.json" ]]; then
        rm "${PROJECT_ROOT}/compile_commands.json"
    fi
}

do_install() {
    log_section "Installing to ${INSTALL_PREFIX}"

    if [[ ! -f "${BUILD_DIR}/CMakeCache.txt" ]]; then
        die "Project not built. Run: $0 build"
    fi

    cmake --install "${BUILD_DIR}"

    log_info "Installation complete"
}

do_all() {
    check_prerequisites

    if [[ "${CLEAN_FIRST}" == true ]]; then
        do_clean
    fi

    do_configure
    do_build
    do_test
}

#######################################
# Usage and Argument Parsing
#######################################

show_usage() {
    grep '^#' "$0" | grep -v '#!/' | sed 's/^# \?//'
}

parse_args() {
    local positional=()

    while [[ $# -gt 0 ]]; do
        case "$1" in
            -t|--type)
                BUILD_TYPE="$2"
                shift 2
                ;;
            -g|--generator)
                case "$2" in
                    make)  GENERATOR="Unix Makefiles" ;;
                    ninja) GENERATOR="Ninja" ;;
                    *)     GENERATOR="$2" ;;
                esac
                shift 2
                ;;
            -j|--jobs)
                PARALLEL_JOBS="$2"
                shift 2
                ;;
            -p|--prefix)
                INSTALL_PREFIX="$2"
                shift 2
                ;;
            -c|--clean)
                CLEAN_FIRST=true
                shift
                ;;
            -v|--verbose)
                VERBOSE=true
                shift
                ;;
            -h|--help)
                show_usage
                exit 0
                ;;
            -*)
                die "Unknown option: $1"
                ;;
            *)
                positional+=("$1")
                shift
                ;;
        esac
    done

    # Restore positional arguments
    set -- "${positional[@]:-}"

    # Determine command
    COMMAND="${1:-all}"
}

#######################################
# Main
#######################################

main() {
    parse_args "$@"

    # Validate build type
    case "${BUILD_TYPE}" in
        Debug|Release|RelWithDebInfo|MinSizeRel) ;;
        *) die "Invalid build type: ${BUILD_TYPE}" ;;
    esac

    # Update build directory based on build type
    BUILD_DIR="${PROJECT_ROOT}/build/${BUILD_TYPE,,}"

    log_info "Project root: ${PROJECT_ROOT}"
    log_info "Build directory: ${BUILD_DIR}"
    log_info "Build type: ${BUILD_TYPE}"

    case "${COMMAND}" in
        configure) check_prerequisites; do_configure ;;
        build)     do_build ;;
        test)      do_test ;;
        clean)     do_clean ;;
        install)   do_install ;;
        all)       do_all ;;
        *)         die "Unknown command: ${COMMAND}" ;;
    esac

    log_info "Done!"
}

main "$@"
```

### 4.3 Multi-Platform Build Script

```bash
#!/bin/bash
#
# cross-build.sh - Cross-platform build script
#

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Detect platform
detect_platform() {
    case "$(uname -s)" in
        Linux*)   echo "linux" ;;
        Darwin*)  echo "macos" ;;
        MINGW*|MSYS*|CYGWIN*) echo "windows" ;;
        *)        echo "unknown" ;;
    esac
}

# Detect architecture
detect_arch() {
    case "$(uname -m)" in
        x86_64|amd64)  echo "x64" ;;
        arm64|aarch64) echo "arm64" ;;
        armv7l)        echo "arm" ;;
        i386|i686)     echo "x86" ;;
        *)             echo "unknown" ;;
    esac
}

PLATFORM=$(detect_platform)
ARCH=$(detect_arch)

echo "Platform: ${PLATFORM}"
echo "Architecture: ${ARCH}"

# Platform-specific settings
case "${PLATFORM}" in
    linux)
        export CC=${CC:-gcc}
        export CXX=${CXX:-g++}
        GENERATOR="Unix Makefiles"
        [[ -x "$(command -v ninja)" ]] && GENERATOR="Ninja"
        ;;
    macos)
        export CC=${CC:-clang}
        export CXX=${CXX:-clang++}
        GENERATOR="Unix Makefiles"
        [[ -x "$(command -v ninja)" ]] && GENERATOR="Ninja"

        # macOS specific: set deployment target
        export MACOSX_DEPLOYMENT_TARGET=${MACOSX_DEPLOYMENT_TARGET:-10.15}
        ;;
    windows)
        # Use Visual Studio or MinGW
        if [[ -n "${MSVC:-}" ]]; then
            GENERATOR="Visual Studio 17 2022"
        else
            GENERATOR="MinGW Makefiles"
        fi
        ;;
    *)
        echo "Unsupported platform: ${PLATFORM}"
        exit 1
        ;;
esac

BUILD_DIR="${SCRIPT_DIR}/build/${PLATFORM}-${ARCH}"

echo "Generator: ${GENERATOR}"
echo "Build directory: ${BUILD_DIR}"

mkdir -p "${BUILD_DIR}"

cmake -S "${SCRIPT_DIR}" \
      -B "${BUILD_DIR}" \
      -G "${GENERATOR}" \
      -DCMAKE_BUILD_TYPE=Release

cmake --build "${BUILD_DIR}" --parallel

echo "Build complete: ${BUILD_DIR}"
```

---

## Part 5: Real-World Build Script Examples

### 5.1 CI/CD Build Script

```bash
#!/bin/bash
#
# ci-build.sh - Build script for CI/CD pipelines
#

set -euo pipefail

# CI-specific configuration
CI=${CI:-false}
BUILD_NUMBER=${BUILD_NUMBER:-local}
GIT_COMMIT=${GIT_COMMIT:-$(git rev-parse --short HEAD 2>/dev/null || echo "unknown")}

PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"
ARTIFACTS_DIR="${PROJECT_ROOT}/artifacts"

log() { echo "[$(date '+%Y-%m-%d %H:%M:%S')] $*"; }

setup_environment() {
    log "Setting up environment..."
    log "  CI: ${CI}"
    log "  Build: ${BUILD_NUMBER}"
    log "  Commit: ${GIT_COMMIT}"

    mkdir -p "${BUILD_DIR}" "${ARTIFACTS_DIR}"
}

build() {
    log "Configuring..."
    cmake -S "${PROJECT_ROOT}" \
          -B "${BUILD_DIR}" \
          -DCMAKE_BUILD_TYPE=Release \
          -DBUILD_NUMBER="${BUILD_NUMBER}" \
          -DGIT_COMMIT="${GIT_COMMIT}"

    log "Building..."
    cmake --build "${BUILD_DIR}" --parallel
}

test() {
    log "Running tests..."
    cd "${BUILD_DIR}"

    # Generate test results in JUnit format for CI
    ctest --output-on-failure \
          --output-junit "${ARTIFACTS_DIR}/test-results.xml"
}

package() {
    log "Packaging..."

    # Create tarball
    local package_name="mycompiler-${BUILD_NUMBER}-${GIT_COMMIT}"
    local package_dir="${ARTIFACTS_DIR}/${package_name}"

    mkdir -p "${package_dir}"
    cp "${BUILD_DIR}/mycompiler" "${package_dir}/"
    cp "${PROJECT_ROOT}/README.md" "${package_dir}/"

    tar -czf "${ARTIFACTS_DIR}/${package_name}.tar.gz" \
        -C "${ARTIFACTS_DIR}" \
        "${package_name}"

    log "Package created: ${ARTIFACTS_DIR}/${package_name}.tar.gz"
}

cleanup() {
    log "Cleaning up..."
    rm -rf "${BUILD_DIR}"
}

main() {
    trap cleanup EXIT

    setup_environment
    build
    test
    package

    log "CI build complete!"
}

main "$@"
```

### 5.2 Development Build Script with Options

```bash
#!/bin/bash
#
# dev-build.sh - Development build with various configurations
#

set -euo pipefail

PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"

# Default configuration
BUILD_TYPE="Debug"
SANITIZER=""
COVERAGE=false
STATIC_ANALYSIS=false

usage() {
    cat << EOF
Usage: $0 [options]

Options:
    --release           Release build
    --debug             Debug build (default)
    --asan              Enable AddressSanitizer
    --tsan              Enable ThreadSanitizer
    --ubsan             Enable UndefinedBehaviorSanitizer
    --coverage          Enable code coverage
    --analyze           Run static analysis
    --all-checks        Enable all sanitizers and analysis
    -h, --help          Show this help

Examples:
    $0 --debug --asan
    $0 --release
    $0 --all-checks
EOF
}

parse_args() {
    while [[ $# -gt 0 ]]; do
        case "$1" in
            --release)      BUILD_TYPE="Release" ;;
            --debug)        BUILD_TYPE="Debug" ;;
            --asan)         SANITIZER="address" ;;
            --tsan)         SANITIZER="thread" ;;
            --ubsan)        SANITIZER="undefined" ;;
            --coverage)     COVERAGE=true ;;
            --analyze)      STATIC_ANALYSIS=true ;;
            --all-checks)
                SANITIZER="address,undefined"
                COVERAGE=true
                STATIC_ANALYSIS=true
                ;;
            -h|--help)
                usage
                exit 0
                ;;
            *)
                echo "Unknown option: $1"
                usage
                exit 1
                ;;
        esac
        shift
    done
}

build() {
    local build_dir="${PROJECT_ROOT}/build/${BUILD_TYPE,,}"
    [[ -n "${SANITIZER}" ]] && build_dir+="-${SANITIZER//,/-}"
    [[ "${COVERAGE}" == true ]] && build_dir+="-coverage"

    echo "Build directory: ${build_dir}"
    mkdir -p "${build_dir}"

    local cmake_args=(
        -S "${PROJECT_ROOT}"
        -B "${build_dir}"
        -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"
    )

    # Sanitizer flags
    if [[ -n "${SANITIZER}" ]]; then
        cmake_args+=(-DCMAKE_CXX_FLAGS="-fsanitize=${SANITIZER}")
        cmake_args+=(-DCMAKE_EXE_LINKER_FLAGS="-fsanitize=${SANITIZER}")
    fi

    # Coverage flags
    if [[ "${COVERAGE}" == true ]]; then
        cmake_args+=(-DCMAKE_CXX_FLAGS="--coverage")
        cmake_args+=(-DCMAKE_EXE_LINKER_FLAGS="--coverage")
    fi

    # Configure
    cmake "${cmake_args[@]}"

    # Static analysis
    if [[ "${STATIC_ANALYSIS}" == true ]]; then
        echo "Running static analysis..."
        cmake --build "${build_dir}" --target clean || true

        # Use clang-tidy if available
        if command -v clang-tidy &>/dev/null; then
            cmake -DCMAKE_CXX_CLANG_TIDY=clang-tidy "${cmake_args[@]}"
        fi
    fi

    # Build
    cmake --build "${build_dir}" --parallel

    echo "Build complete: ${build_dir}"
}

parse_args "$@"
build
```

### 5.3 Complete Project Build Script Template

```bash
#!/bin/bash
#######################################
# Project Build Script
#
# A comprehensive build script template
# for CMake-based C++ projects.
#######################################

set -euo pipefail

#######################################
# Constants
#######################################
readonly SCRIPT_NAME="$(basename "$0")"
readonly SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly PROJECT_ROOT="${SCRIPT_DIR}"

# Version info
readonly VERSION="1.0.0"

#######################################
# Default Configuration
#######################################
BUILD_TYPE="Release"
BUILD_DIR=""
INSTALL_PREFIX="/usr/local"
GENERATOR=""
PARALLEL_JOBS=""
VERBOSE=false
CLEAN_BUILD=false
RUN_TESTS=false
DO_INSTALL=false

# Feature flags
ENABLE_TESTS=true
ENABLE_DOCS=false
ENABLE_COVERAGE=false
SANITIZER=""

#######################################
# Colors (auto-detect terminal support)
#######################################
setup_colors() {
    if [[ -t 1 ]] && [[ "${TERM:-}" != "dumb" ]]; then
        RED='\033[0;31m'
        GREEN='\033[0;32m'
        YELLOW='\033[1;33m'
        BLUE='\033[0;34m'
        MAGENTA='\033[0;35m'
        CYAN='\033[0;36m'
        BOLD='\033[1m'
        NC='\033[0m'
    else
        RED='' GREEN='' YELLOW='' BLUE=''
        MAGENTA='' CYAN='' BOLD='' NC=''
    fi
}

#######################################
# Logging
#######################################
log_info()    { echo -e "${GREEN}[INFO]${NC} $*"; }
log_warn()    { echo -e "${YELLOW}[WARN]${NC} $*" >&2; }
log_error()   { echo -e "${RED}[ERROR]${NC} $*" >&2; }
log_debug()   { [[ "${VERBOSE}" == true ]] && echo -e "${CYAN}[DEBUG]${NC} $*" || true; }
log_section() { echo -e "\n${BOLD}${BLUE}==>${NC} ${BOLD}$*${NC}"; }

die() {
    log_error "$1"
    exit "${2:-1}"
}

#######################################
# System Detection
#######################################
detect_system() {
    # Detect OS
    case "$(uname -s)" in
        Linux*)   OS="linux" ;;
        Darwin*)  OS="macos" ;;
        MINGW*|MSYS*|CYGWIN*) OS="windows" ;;
        *)        OS="unknown" ;;
    esac

    # Detect architecture
    case "$(uname -m)" in
        x86_64|amd64)  ARCH="x64" ;;
        arm64|aarch64) ARCH="arm64" ;;
        armv7*)        ARCH="arm" ;;
        i?86)          ARCH="x86" ;;
        *)             ARCH="unknown" ;;
    esac

    # Detect CPU cores
    if command -v nproc &>/dev/null; then
        CPU_CORES=$(nproc)
    elif command -v sysctl &>/dev/null; then
        CPU_CORES=$(sysctl -n hw.ncpu)
    else
        CPU_CORES=4
    fi

    # Set defaults based on system
    PARALLEL_JOBS="${PARALLEL_JOBS:-${CPU_CORES}}"

    # Detect best generator
    if [[ -z "${GENERATOR}" ]]; then
        if command -v ninja &>/dev/null; then
            GENERATOR="Ninja"
        else
            GENERATOR="Unix Makefiles"
        fi
    fi

    log_debug "System: ${OS}-${ARCH}, ${CPU_CORES} cores"
    log_debug "Generator: ${GENERATOR}"
}

#######################################
# Prerequisite Checks
#######################################
check_prerequisites() {
    log_section "Checking prerequisites"

    local missing=()

    # Required tools
    command -v cmake &>/dev/null || missing+=("cmake")

    # C++ compiler
    if ! command -v g++ &>/dev/null && ! command -v clang++ &>/dev/null; then
        missing+=("C++ compiler (g++ or clang++)")
    fi

    # Generator-specific
    if [[ "${GENERATOR}" == "Ninja" ]]; then
        command -v ninja &>/dev/null || missing+=("ninja")
    fi

    # Optional tools
    if [[ "${ENABLE_DOCS}" == true ]]; then
        command -v doxygen &>/dev/null || log_warn "doxygen not found, documentation will be skipped"
    fi

    if [[ ${#missing[@]} -gt 0 ]]; then
        die "Missing required tools: ${missing[*]}"
    fi

    # Version checks
    local cmake_version
    cmake_version=$(cmake --version | head -1 | grep -oE '[0-9]+\.[0-9]+\.[0-9]+')
    log_info "CMake version: ${cmake_version}"

    # Check minimum version
    if [[ "$(printf '%s\n' "3.16" "${cmake_version}" | sort -V | head -n1)" != "3.16" ]]; then
        die "CMake 3.16 or higher required (found ${cmake_version})"
    fi

    log_info "All prerequisites satisfied"
}

#######################################
# Build Functions
#######################################
configure() {
    log_section "Configuring (${BUILD_TYPE})"

    mkdir -p "${BUILD_DIR}"

    local cmake_args=(
        -S "${PROJECT_ROOT}"
        -B "${BUILD_DIR}"
        -G "${GENERATOR}"
        -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"
        -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}"
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
        -DENABLE_TESTS="${ENABLE_TESTS}"
    )

    # Optional features
    [[ "${ENABLE_COVERAGE}" == true ]] && cmake_args+=(-DENABLE_COVERAGE=ON)
    [[ -n "${SANITIZER}" ]] && cmake_args+=(-DSANITIZER="${SANITIZER}")

    # Verbose mode
    [[ "${VERBOSE}" == true ]] && cmake_args+=(-DCMAKE_VERBOSE_MAKEFILE=ON)

    log_debug "cmake ${cmake_args[*]}"
    cmake "${cmake_args[@]}"

    # Symlink compile_commands.json for IDE support
    if [[ -f "${BUILD_DIR}/compile_commands.json" ]]; then
        ln -sf "${BUILD_DIR}/compile_commands.json" "${PROJECT_ROOT}/"
    fi

    log_info "Configuration complete"
}

build() {
    log_section "Building"

    [[ -f "${BUILD_DIR}/CMakeCache.txt" ]] || configure

    local build_args=(
        --build "${BUILD_DIR}"
        --parallel "${PARALLEL_JOBS}"
    )

    [[ "${VERBOSE}" == true ]] && build_args+=(--verbose)

    local start_time=$(date +%s)
    cmake "${build_args[@]}"
    local end_time=$(date +%s)

    log_info "Build completed in $((end_time - start_time))s"
}

run_tests() {
    log_section "Running tests"

    [[ -f "${BUILD_DIR}/CMakeCache.txt" ]] || die "Project not built"

    cd "${BUILD_DIR}"
    ctest --output-on-failure --parallel "${PARALLEL_JOBS}"

    log_info "All tests passed"
}

install() {
    log_section "Installing to ${INSTALL_PREFIX}"

    [[ -f "${BUILD_DIR}/CMakeCache.txt" ]] || die "Project not built"

    cmake --install "${BUILD_DIR}"

    log_info "Installation complete"
}

clean() {
    log_section "Cleaning"

    [[ -d "${BUILD_DIR}" ]] && rm -rf "${BUILD_DIR}"
    [[ -L "${PROJECT_ROOT}/compile_commands.json" ]] && rm -f "${PROJECT_ROOT}/compile_commands.json"

    log_info "Clean complete"
}

#######################################
# Usage
#######################################
usage() {
    cat << EOF
${BOLD}${SCRIPT_NAME}${NC} v${VERSION} - Build script for the project

${BOLD}USAGE:${NC}
    ${SCRIPT_NAME} [OPTIONS] [COMMAND]

${BOLD}COMMANDS:${NC}
    configure       Configure the project
    build           Build the project (default)
    test            Run tests
    install         Install the project
    clean           Clean build directory
    all             Configure, build, and test

${BOLD}OPTIONS:${NC}
    -t, --type TYPE       Build type: Debug, Release, RelWithDebInfo, MinSizeRel
                          (default: Release)
    -g, --generator GEN   CMake generator: make, ninja, or full name
                          (default: auto-detect)
    -j, --jobs N          Parallel jobs (default: auto-detect)
    -p, --prefix PATH     Install prefix (default: /usr/local)
    -c, --clean           Clean before building
    -v, --verbose         Verbose output

    --with-tests          Enable tests (default: on)
    --without-tests       Disable tests
    --with-coverage       Enable code coverage
    --sanitizer TYPE      Enable sanitizer: address, thread, undefined

    -h, --help            Show this help
    --version             Show version

${BOLD}EXAMPLES:${NC}
    ${SCRIPT_NAME}                          # Build release
    ${SCRIPT_NAME} --type Debug             # Build debug
    ${SCRIPT_NAME} --clean build test       # Clean build and test
    ${SCRIPT_NAME} --sanitizer address      # Build with ASan
    ${SCRIPT_NAME} --type Debug --coverage  # Debug with coverage

${BOLD}ENVIRONMENT:${NC}
    CC                    C compiler
    CXX                   C++ compiler
    CMAKE_BUILD_PARALLEL_LEVEL  Default parallel jobs

EOF
}

#######################################
# Argument Parsing
#######################################
parse_args() {
    local commands=()

    while [[ $# -gt 0 ]]; do
        case "$1" in
            -t|--type)
                BUILD_TYPE="$2"
                shift 2
                ;;
            -g|--generator)
                case "$2" in
                    make)   GENERATOR="Unix Makefiles" ;;
                    ninja)  GENERATOR="Ninja" ;;
                    *)      GENERATOR="$2" ;;
                esac
                shift 2
                ;;
            -j|--jobs)
                PARALLEL_JOBS="$2"
                shift 2
                ;;
            -p|--prefix)
                INSTALL_PREFIX="$2"
                shift 2
                ;;
            -c|--clean)
                CLEAN_BUILD=true
                shift
                ;;
            -v|--verbose)
                VERBOSE=true
                shift
                ;;
            --with-tests)
                ENABLE_TESTS=true
                shift
                ;;
            --without-tests)
                ENABLE_TESTS=false
                shift
                ;;
            --with-coverage)
                ENABLE_COVERAGE=true
                shift
                ;;
            --sanitizer)
                SANITIZER="$2"
                shift 2
                ;;
            -h|--help)
                usage
                exit 0
                ;;
            --version)
                echo "${VERSION}"
                exit 0
                ;;
            -*)
                die "Unknown option: $1"
                ;;
            *)
                commands+=("$1")
                shift
                ;;
        esac
    done

    # Default command
    [[ ${#commands[@]} -eq 0 ]] && commands=("build")

    # Store commands
    COMMANDS=("${commands[@]}")
}

#######################################
# Main
#######################################
main() {
    setup_colors
    parse_args "$@"
    detect_system

    # Validate build type
    case "${BUILD_TYPE}" in
        Debug|Release|RelWithDebInfo|MinSizeRel) ;;
        *) die "Invalid build type: ${BUILD_TYPE}" ;;
    esac

    # Set build directory
    BUILD_DIR="${BUILD_DIR:-${PROJECT_ROOT}/build/${BUILD_TYPE,,}}"
    [[ -n "${SANITIZER}" ]] && BUILD_DIR+="-${SANITIZER}"

    # Print configuration
    log_info "Project: ${PROJECT_ROOT}"
    log_info "Build: ${BUILD_DIR}"
    log_info "Type: ${BUILD_TYPE}"

    # Clean if requested
    [[ "${CLEAN_BUILD}" == true ]] && clean

    # Execute commands
    for cmd in "${COMMANDS[@]}"; do
        case "${cmd}" in
            configure)  check_prerequisites; configure ;;
            build)      build ;;
            test)       run_tests ;;
            install)    install ;;
            clean)      clean ;;
            all)        check_prerequisites; configure; build; run_tests ;;
            *)          die "Unknown command: ${cmd}" ;;
        esac
    done

    echo ""
    log_info "Done!"
}

main "$@"
```

---

## Summary

### Key Shell Script Concepts

| Concept | Syntax |
|---------|--------|
| Variables | `VAR="value"`, `${VAR}` |
| Command substitution | `$(command)` |
| Conditionals | `if [[ condition ]]; then ... fi` |
| Loops | `for x in list; do ... done` |
| Functions | `func() { ... }` |
| Arguments | `$1`, `$@`, `$#` |
| Exit status | `$?`, `exit 1` |
| Error handling | `set -euo pipefail` |

### Key CMake Concepts

| Concept | Command |
|---------|---------|
| Minimum version | `cmake_minimum_required(VERSION 3.16)` |
| Project | `project(Name VERSION 1.0)` |
| Executable | `add_executable(name sources...)` |
| Library | `add_library(name STATIC sources...)` |
| Link | `target_link_libraries(target lib)` |
| Include dirs | `target_include_directories(target dir)` |
| Compile options | `target_compile_options(target flags)` |
| Install | `install(TARGETS target DESTINATION dir)` |

### Build Script Best Practices

1. **Always use `set -euo pipefail`** for error handling
2. **Detect system automatically** (OS, architecture, cores)
3. **Provide sensible defaults** that can be overridden
4. **Use colored output** for better readability
5. **Include usage/help documentation**
6. **Support both configure and build steps**
7. **Create compile_commands.json** for IDE support
8. **Measure and report build times**
9. **Handle cleanup properly** with traps
10. **Support CI/CD environments** with appropriate flags
