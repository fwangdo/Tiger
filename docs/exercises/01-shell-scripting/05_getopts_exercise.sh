#!/bin/bash
# Exercise 5: Argument Parsing with getopts
#
# Learn to parse command-line options.
#
# Run: ./05_getopts_exercise.sh -t Debug -j 4 -v input.txt
# Expected:
#   Build type: Debug
#   Jobs: 4
#   Verbose: true
#   Input file: input.txt

# Default values
BUILD_TYPE="Release"
JOBS=1
VERBOSE=false

usage() {
    cat << EOF
Usage: $0 [options] <input_file>

Options:
    -t TYPE    Build type (Debug/Release), default: Release
    -j N       Number of parallel jobs, default: 1
    -v         Enable verbose output
    -h         Show this help
EOF
    exit 0
}

# TODO 1: Complete the getopts option string
# Options: t (with argument), j (with argument), v (flag), h (flag)
# Hint: Options with arguments have : after them
while getopts "___" opt; do
    case $opt in
        # TODO 2: Handle -t option (set BUILD_TYPE to $OPTARG)
        t)
            ___
            ;;
        # TODO 3: Handle -j option (set JOBS to $OPTARG)
        j)
            ___
            ;;
        # TODO 4: Handle -v option (set VERBOSE to true)
        v)
            ___
            ;;
        h)
            usage
            ;;
        # TODO 5: Handle invalid option (print usage and exit)
        \?)
            ___
            ;;
    esac
done

# TODO 6: Shift processed options to get remaining arguments
# Hint: Use shift $((OPTIND - 1))
___

# TODO 7: Get the input file from remaining arguments ($1)
# If not provided, print usage
INPUT_FILE="${1:-}"
if [ -z "$INPUT_FILE" ]; then
    echo "Error: Input file required" >&2
    usage
fi

# Print results
echo "Build type: $BUILD_TYPE"
echo "Jobs: $JOBS"
echo "Verbose: $VERBOSE"
echo "Input file: $INPUT_FILE"
