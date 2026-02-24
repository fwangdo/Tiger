# CMakeLists.txt for Tiger Compiler
# This serves as a practical example of CMake usage

cmake_minimum_required(VERSION 3.16)

project(TigerCompiler
    VERSION 1.0.0
    DESCRIPTION "Tiger language compiler"
    LANGUAGES CXX
)
#######################################
# C++ Standard
#######################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#######################################
# Build Type
#######################################
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "you should build and config types. ")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

#######################################
# Options
#######################################
option(ENABLE_TEST "Build test" ON)
option(ENABLE_WARNINGS "Enable compiler warnings" ON)
option(ENABLE_SANITIZERS "Enable Sanitizer" OFF)

#######################################
# Compiler Warnings
#######################################
if (ENABLE_WARNINGS) 
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clnag")
    add_compile_options(-Wall -Wextra -Wpedantic)
  elseif(MSVC)
    add_compile_options(/W4)
  endif()
endif()

#######################################
# Sanitizers (Debug only)
#######################################
# TODO 

#######################################
# Library: tiger_core
#######################################
# STATIC Integrate object files to one file(.a)
# SHARED Make the files in dynamic lib. 
# OBJECT only generate object. 
add_library(tiger_core STATIC
  src/lexer/Token.cpp 
  src/lexer/Lexer.cpp
  src/parser/Parser.cpp
  src/util/ASTPrinter.cpp
  src/env/EnvTable.cpp
  src/env/symbol.cpp
)

target_include_directories(tiger_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

# Add debug info for core library
target_compile_definitions(tiger_core PRIVATE
    $<$<CONFIG:Debug>:TIGER_DEBUG>
)

#######################################
# Executable: tiger
#######################################
add_executable(tiger src/main.cpp)
target_link_libraries(tiger PRIVATE tiger_core)

#######################################
# Installation
#######################################

install(TARGETS tiger
    RUNTIME DESTINATION bin
)

install(TARGETS tiger_core
    ARCHIVE DESTINATION lib
)

install(FILES
    src/lexer/Token.hpp
    src/lexer/Lexer.hpp
    src/parser/AST.hpp
    src/parser/Parser.hpp
    src/util/ASTPrinter.hpp
    DESTINATION include/tiger
)

#######################################
# Testing (CTest)
#######################################

#######################################
# Export compile_commands.json
#######################################

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
