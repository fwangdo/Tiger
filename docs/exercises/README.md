# Compiler Construction Exercises

Hands-on exercises for learning to build compilers and build systems.

## Structure

```
exercises/
├── 01-shell-scripting/    # Shell scripting fundamentals (5 exercises)
├── 02-cmake/              # CMake build system (2 exercises)
├── 03-lexer/              # Lexical analysis (2 exercises)
├── 04-parser/             # Recursive descent parsing (2 exercises)
└── 05-ast/                # Abstract Syntax Tree design (1 exercise)
```

## Exercise Format

Each topic has:
- `README.md` - Theory and quick reference
- `*_exercise.*` - Fill in the `___` placeholders
- `*_solution.*` - Reference solution

## Quick Start

```bash
# 1. Shell scripting
cd 01-shell-scripting
./01_variables_solution.sh    # See expected output
# Edit 01_variables_exercise.sh, fill in ___ blanks
./01_variables_exercise.sh    # Test your solution

# 2. CMake
cd 02-cmake/01-basic
cp CMakeLists_exercise.txt CMakeLists.txt
# Edit CMakeLists.txt, fill in blanks
mkdir build && cd build && cmake .. && make

# 3. C++ exercises (lexer/parser/ast)
cd 03-lexer
g++ -std=c++17 -o test 02_scan_identifier_solution.cpp && ./test
# Edit 02_scan_identifier_exercise.cpp
g++ -std=c++17 -o test 02_scan_identifier_exercise.cpp && ./test
```

## Topics by Difficulty

### Easy
- `01-shell-scripting/01_variables` - Variables
- `01-shell-scripting/02_conditionals` - If/else
- `01-shell-scripting/03_loops` - For/while loops
- `02-cmake/01-basic` - Basic CMakeLists.txt
- `03-lexer/01_token_types` - Token enum/struct
- `04-parser/01_token_handling` - peek/match/expect

### Medium
- `01-shell-scripting/04_functions` - Functions
- `01-shell-scripting/05_getopts` - Argument parsing
- `02-cmake/02-library` - Library + executable
- `03-lexer/02_scan_identifier` - Identifier scanning
- `04-parser/02_precedence` - Operator precedence
- `05-ast/01_basic_nodes` - AST node design
