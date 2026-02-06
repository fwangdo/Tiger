# CMake Exercises

Learn to write CMake build configurations.

## Topics Covered

1. Basic CMakeLists.txt structure
2. Creating executables and libraries
3. Build types and options
4. Compiler flags and definitions
5. Installing targets

## Exercises

| Directory | Topic | Difficulty |
|-----------|-------|------------|
| `01-basic/` | Minimal CMakeLists.txt | Easy |
| `02-library/` | Static library + executable | Easy |
| `03-options/` | Build types and options | Medium |
| `04-install/` | Installation rules | Medium |
| `05-complete/` | Full project structure | Hard |

## How to Build

```bash
cd 01-basic
mkdir build && cd build
cmake ..
cmake --build .
```

## Quick Reference

### Minimum Template
```cmake
cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(myapp main.cpp)
```

### Common Commands
```cmake
add_executable(name sources...)
add_library(name STATIC sources...)
target_link_libraries(target PRIVATE lib)
target_include_directories(target PRIVATE dir)
target_compile_options(target PRIVATE -Wall)
target_compile_definitions(target PRIVATE DEBUG)
```

### Variables
```cmake
${CMAKE_SOURCE_DIR}      # Top-level source dir
${CMAKE_BINARY_DIR}      # Top-level build dir
${CMAKE_BUILD_TYPE}      # Debug, Release, etc.
${PROJECT_NAME}          # Project name
```
