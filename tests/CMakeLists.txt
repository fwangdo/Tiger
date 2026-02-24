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
    message(STATUS "Setting build type to 'Release' as none was specified")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

#######################################
# Options
#######################################
option(ENABLE_TESTS "Build tests" ON)
option(ENABLE_WARNINGS "Enable compiler warnings" ON)
option(ENABLE_SANITIZERS "Enable AddressSanitizer and UBSan in Debug" OFF)

#######################################
# Compiler Warnings
#######################################
if(ENABLE_WARNINGS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-Wall -Wextra -Wpedantic)
    elseif(MSVC)
        add_compile_options(/W4)
    endif()
endif()

#######################################
# Sanitizers (Debug only)
#######################################
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined)
        message(STATUS "Sanitizers enabled")
    endif()
endif()

#######################################
# Library: tiger_core
#######################################
# library 
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
if(ENABLE_TESTS)
    enable_testing()

    # Test: Lexer tokenizes correctly
    add_test(
        NAME test_lexer_basic
        COMMAND tiger --lex ${CMAKE_SOURCE_DIR}/tests/basic.tig
    )

    # Test: Parser parses correctly
    add_test(
        NAME test_parser_basic
        COMMAND tiger --parse ${CMAKE_SOURCE_DIR}/tests/basic.tig
    )

    # Test: AST output
    add_test(
        NAME test_ast_basic
        COMMAND tiger --ast ${CMAKE_SOURCE_DIR}/tests/basic.tig
    )

    add_executable(test_env_table tests/test_env_table.cpp)
    target_link_libraries(test_env_table PRIVATE tiger_core)

    # Test; Environment. 
    add_test(
        NAME test_env_table        
        COMMAND test_env_table 
    )

endif()

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
