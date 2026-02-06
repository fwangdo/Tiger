# Lexer Exercises

Learn to implement a lexical analyzer (tokenizer).

## Topics Covered

1. Token types and structures
2. Character-level operations (peek, advance)
3. Scanning identifiers and keywords
4. Scanning numbers
5. Scanning strings with escape sequences
6. Multi-character operators

## Exercises

| File | Topic | Difficulty |
|------|-------|------------|
| `01_token_types` | Define token enum and struct | Easy |
| `02_char_operations` | Implement peek/advance | Easy |
| `03_scan_identifier` | Scan identifiers and keywords | Medium |
| `04_scan_number` | Scan integer literals | Easy |
| `05_scan_string` | Scan strings with escapes | Medium |
| `06_scan_operators` | Multi-character operators | Medium |
| `07_complete_lexer` | Full lexer implementation | Hard |

## How to Build

```bash
# Single exercise
g++ -std=c++17 -o test 03_scan_identifier_exercise.cpp
./test

# With the provided Makefile
make test_03
```

## Key Concepts

### Lexer Pattern
```cpp
class Lexer {
    std::string source_;
    size_t pos_;

    char peek() { return source_[pos_]; }
    char advance() { return source_[pos_++]; }
    bool at_end() { return pos_ >= source_.size(); }

    Token next_token();
};
```

### Token Structure
```cpp
struct Token {
    TokenType type;
    std::string text;
    int line, column;
};
```
