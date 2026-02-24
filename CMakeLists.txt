# CMakeLists.txt for Tiger Compiler
# This serves as a practical example of CMake usage


#######################################
# C++ Standard
#######################################


#######################################
# Build Type
#######################################

#######################################
# Options
#######################################

#######################################
# Compiler Warnings
#######################################

#######################################
# Sanitizers (Debug only)
#######################################

#######################################
# Library: tiger_core
#######################################


#######################################
# Executable: tiger
#######################################

#######################################
# Installation
#######################################

#######################################
# Testing (CTest)
#######################################

#######################################
# Export compile_commands.json
#######################################

#######################################
# Summary
#######################################
message(STATUS "")
message(STATUS "=== Tiger Compiler Configuration ===")
message(STATUS "Version:      ${PROJECT_VERSION}")
message(STATUS "Build type:   ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Warnings:     ${ENABLE_WARNINGS}")
message(STATUS "Sanitizers:   ${ENABLE_SANITIZERS}")
message(STATUS "Tests:        ${ENABLE_TESTS}")
message(STATUS "Install:      ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
