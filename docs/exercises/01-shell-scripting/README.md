# Shell Scripting Exercises

Learn shell scripting fundamentals for build automation.

## Topics Covered

1. Variables and command substitution
2. Conditionals and file tests
3. Loops (for, while)
4. Functions
5. Argument parsing with getopts
6. Error handling

## Exercises

| File | Topic | Difficulty |
|------|-------|------------|
| `01_variables` | Variables and substitution | Easy |
| `02_conditionals` | if/else and test operators | Easy |
| `03_loops` | for and while loops | Easy |
| `04_functions` | Function definitions | Medium |
| `05_getopts` | Argument parsing | Medium |
| `06_error_handling` | set -e, traps | Medium |
| `07_build_script` | Complete build script | Hard |

## How to Run

```bash
# Run exercise
./01_variables_exercise.sh

# Compare with solution
./01_variables_solution.sh

# Run all tests
./run_tests.sh
```

## Quick Reference

### Variables
```bash
VAR="value"           # Assignment (no spaces!)
echo "$VAR"           # Use variable
result=$(command)     # Command substitution
${VAR:-default}       # Default value
```

### Conditionals
```bash
if [ -f "$FILE" ]; then    # File exists
if [ -d "$DIR" ]; then     # Directory exists
if [ -z "$STR" ]; then     # String is empty
if [ "$A" -eq "$B" ]; then # Numbers equal
```

### Loops
```bash
for item in list; do ... done
for ((i=0; i<5; i++)); do ... done
while [ condition ]; do ... done
```

### Functions
```bash
my_func() {
    local var="$1"
    echo "$var"
}
result=$(my_func "arg")
```
