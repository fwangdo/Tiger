#!/bin/bash
#
# build.sh - Build script for the Tiger compiler
#
# Usage: ./build.sh [options] [command]
#
# Commands:
#   configure   Configure the project with CMake
#   build       Build the project (default)
#   test        Run tests
#   clean       Clean build artifacts
#   install     Install to prefix
#   all         Configure, build, and test
#
# Options:
#   -t, --type TYPE     Build type: Debug, Release (default: Release)
#   -g, --generator GEN Generator: make, ninja (default: auto)
#   -j, --jobs N        Parallel jobs (default: auto)
#   -p, --prefix PATH   Install prefix (default: /usr/local)
#   -c, --clean         Clean before building
#   -s, --sanitizers    Enable sanitizers (Debug only)
#   -v, --verbose       Verbose output
#   -h, --help          Show this help
#
# Examples:
#   ./build.sh                    # Build release
#   ./build.sh --type Debug       # Build debug
#   ./build.sh -t Debug -s all    # Debug with sanitizers, run all
#   ./build.sh --clean build test # Clean, build, and test
#

set -euo pipefail

#######################################
# Configuration
#######################################
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="${SCRIPT_DIR}"

# Build options
BUILD_TYPE="Release"
GENERATOR=""
PARALLEL_JOBS=""
INSTALL_PREFIX="/usr/local"
CLEAN_FIRST=false
ENABLE_SANITIZERS=false
VERBOSE=false

#######################################
# Colors
#######################################
setup_colors() {
    if [[ -t 1 ]]; then
        RED='\033[0;31m'
        GREEN='\033[0;32m'
        YELLOW='\033[1;33m'
        BLUE='\033[0;34m'
        BOLD='\033[1m'
        NC='\033[0m'
    else
        RED='' GREEN='' YELLOW='' BLUE='' BOLD='' NC=''
    fi
}

#######################################
# Logging
#######################################
log_info()    { echo -e "${GREEN}[INFO]${NC} $*"; }
log_warn()    { echo -e "${YELLOW}[WARN]${NC} $*" >&2; }
log_error()   { echo -e "${RED}[ERROR]${NC} $*" >&2; }
log_section() { echo -e "\n${BOLD}${BLUE}==>${NC} ${BOLD}$*${NC}"; }

die() {
    log_error "$1"
    exit "${2:-1}"
}

#######################################
# System Detection
#######################################
detect_system() {
    # Detect CPU cores
    if command -v nproc &>/dev/null; then
        CPU_CORES=$(nproc)
    elif command -v sysctl &>/dev/null; then
        CPU_CORES=$(sysctl -n hw.ncpu 2>/dev/null || echo 4)
    else
        CPU_CORES=4
    fi

    PARALLEL_JOBS="${PARALLEL_JOBS:-${CPU_CORES}}"

    # Detect best generator
    if [[ -z "${GENERATOR}" ]]; then
        if command -v ninja &>/dev/null; then
            GENERATOR="Ninja"
        else
            GENERATOR="Unix Makefiles"
        fi
    fi
}

#######################################
# Prerequisites
#######################################
check_prerequisites() {
    log_section "Checking prerequisites"

    command -v cmake &>/dev/null || die "cmake is required"

    if ! command -v g++ &>/dev/null && ! command -v clang++ &>/dev/null; then
        die "C++ compiler (g++ or clang++) is required"
    fi

    if [[ "${GENERATOR}" == "Ninja" ]]; then
        command -v ninja &>/dev/null || {
            log_warn "Ninja not found, falling back to Make"
            GENERATOR="Unix Makefiles"
        }
    fi

    local cmake_version
    cmake_version=$(cmake --version | head -1 | grep -oE '[0-9]+\.[0-9]+')
    log_info "CMake: ${cmake_version}"
    log_info "Generator: ${GENERATOR}"
    log_info "Jobs: ${PARALLEL_JOBS}"
}

#######################################
# Build Functions
#######################################
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

    if [[ "${ENABLE_SANITIZERS}" == true ]]; then
        cmake_args+=(-DENABLE_SANITIZERS=ON)
    fi

    if [[ "${VERBOSE}" == true ]]; then
        cmake_args+=(-DCMAKE_VERBOSE_MAKEFILE=ON)
    fi

    cmake "${cmake_args[@]}"

    # Symlink compile_commands.json
    if [[ -f "${BUILD_DIR}/compile_commands.json" ]]; then
        ln -sf "${BUILD_DIR}/compile_commands.json" "${PROJECT_ROOT}/"
        log_info "Created compile_commands.json symlink"
    fi

    log_info "Configuration complete"
}

do_build() {
    log_section "Building"

    if [[ ! -f "${BUILD_DIR}/CMakeCache.txt" ]]; then
        log_warn "Not configured, running configure first..."
        do_configure
    fi

    local build_args=(
        --build "${BUILD_DIR}"
        --parallel "${PARALLEL_JOBS}"
    )

    if [[ "${VERBOSE}" == true ]]; then
        build_args+=(--verbose)
    fi

    local start_time
    start_time=$(date +%s)

    cmake "${build_args[@]}"

    local end_time
    end_time=$(date +%s)
    log_info "Build completed in $((end_time - start_time))s"
}

do_test() {
    log_section "Running tests"

    if [[ ! -f "${BUILD_DIR}/CMakeCache.txt" ]]; then
        die "Project not configured. Run: $0 build"
    fi

    cd "${BUILD_DIR}"
    ctest --output-on-failure --parallel "${PARALLEL_JOBS}"

    log_info "All tests passed"
}

do_install() {
    log_section "Installing to ${INSTALL_PREFIX}"

    if [[ ! -f "${BUILD_DIR}/CMakeCache.txt" ]]; then
        die "Project not built. Run: $0 build"
    fi

    cmake --install "${BUILD_DIR}"
    log_info "Installation complete"
}

do_clean() {
    log_section "Cleaning"

    if [[ -d "${BUILD_DIR}" ]]; then
        rm -rf "${BUILD_DIR}"
        log_info "Removed ${BUILD_DIR}"
    fi

    if [[ -L "${PROJECT_ROOT}/compile_commands.json" ]]; then
        rm -f "${PROJECT_ROOT}/compile_commands.json"
    fi

    log_info "Clean complete"
}

do_all() {
    check_prerequisites
    do_configure
    do_build
    do_test
}

#######################################
# Usage
#######################################
show_usage() {
    grep '^#' "$0" | grep -v '#!/' | sed 's/^# \?//' | head -30
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
                CLEAN_FIRST=true
                shift
                ;;
            -s|--sanitizers)
                ENABLE_SANITIZERS=true
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
                commands+=("$1")
                shift
                ;;
        esac
    done

    # Default command
    if [[ ${#commands[@]} -eq 0 ]]; then
        commands=("build")
    fi

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

    # Set build directory (lowercase build type for directory name)
    local build_type_lower
    build_type_lower=$(echo "${BUILD_TYPE}" | tr '[:upper:]' '[:lower:]')
    BUILD_DIR="${PROJECT_ROOT}/build/${build_type_lower}"

    # Print configuration
    echo ""
    log_info "Tiger Compiler Build Script"
    log_info "Project:    ${PROJECT_ROOT}"
    log_info "Build dir:  ${BUILD_DIR}"
    log_info "Build type: ${BUILD_TYPE}"
    log_info "Sanitizers: ${ENABLE_SANITIZERS}"
    echo ""

    # Clean if requested
    if [[ "${CLEAN_FIRST}" == true ]]; then
        do_clean
    fi

    # Execute commands
    for cmd in "${COMMANDS[@]}"; do
        case "${cmd}" in
            configure)  check_prerequisites; do_configure ;;
            build)      do_build ;;
            test)       do_test ;;
            install)    do_install ;;
            clean)      do_clean ;;
            all)        do_all ;;
            *)          die "Unknown command: ${cmd}" ;;
        esac
    done

    echo ""
    log_info "Done!"
}

main "$@"
