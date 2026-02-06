# Parser Exercises

Learn to implement a recursive descent parser.

## Topics Covered

1. Token handling (peek, advance, match, expect)
2. Operator precedence
3. Parsing expressions
4. Parsing control flow (if, while, for)
5. Parsing declarations
6. Error handling

## Exercises

| File | Topic | Difficulty |
|------|-------|------------|
| `01_token_handling` | peek, advance, match, expect | Easy |
| `02_precedence` | Operator precedence parsing | Medium |
| `03_parse_primary` | Primary expressions | Easy |
| `04_parse_if` | If-then-else expressions | Medium |
| `05_parse_while` | While loops | Easy |
| `06_parse_call` | Function calls | Medium |
| `07_parse_let` | Let expressions | Hard |

## How to Build

```bash
g++ -std=c++17 -o test 01_token_handling_exercise.cpp
./test
```

## Key Concepts

### Parser Pattern
```cpp
class Parser {
    Lexer& lexer_;
    Token current_;

    Token peek() { return current_; }
    Token advance() { Token t = current_; current_ = lexer_.next_token(); return t; }
    bool check(TokenType t) { return current_.type == t; }
    bool match(TokenType t) { if (check(t)) { advance(); return true; } return false; }
    void expect(TokenType t, const char* msg);
};
```

### Precedence Climbing
```cpp
// Lower precedence = called first
ExpPtr parse_expression() { return parse_assignment(); }
ExpPtr parse_assignment() { /* ... */ return parse_or(); }
ExpPtr parse_or() { /* ... */ return parse_and(); }
ExpPtr parse_and() { /* ... */ return parse_comparison(); }
ExpPtr parse_comparison() { /* ... */ return parse_additive(); }
ExpPtr parse_additive() { /* ... */ return parse_multiplicative(); }
ExpPtr parse_multiplicative() { /* ... */ return parse_unary(); }
ExpPtr parse_unary() { /* ... */ return parse_primary(); }
```

### Left vs Right Associativity
```cpp
// Left associative: a - b - c = (a - b) - c
while (match(MINUS)) {
    right = parse_higher_precedence();
    left = make_binop(left, right);
}

// Right associative: a = b = c = a = (b = c)
if (match(ASSIGN)) {
    right = parse_same_precedence();  // recursive call
    return make_assign(left, right);
}
```
