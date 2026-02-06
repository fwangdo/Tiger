#!/bin/bash
# Exercise 5: Argument Parsing with getopts - SOLUTION

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
while getopts "t:j:vh" opt; do
    case $opt in
        # TODO 2: Handle -t option
        t)
            BUILD_TYPE="$OPTARG"
            ;;
        # TODO 3: Handle -j option
        j)
            JOBS="$OPTARG"
            ;;
        # TODO 4: Handle -v option
        v)
            VERBOSE=true
            ;;
        h)
            usage
            ;;
        # TODO 5: Handle invalid option
        \?)
            usage
            ;;
    esac
done

# TODO 6: Shift processed options
shift $((OPTIND - 1))

# TODO 7: Get the input file
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
