# Quiz: Building Lexers and Parsers

Test your understanding with these exercises. Each section includes theory questions and code exercises.

---

## Section 1: Build Systems

### Theory Questions

**Q1.1** What is the difference between compiling and linking?

<details>
<summary>Answer</summary>

- **Compiling**: Converts source code (`.cpp`) into object code (`.o`). Each source file is compiled independently.
- **Linking**: Combines multiple object files into a single executable, resolving references between files.

</details>

**Q1.2** In a Makefile, what do `$@`, `$<`, and `$^` represent?

<details>
<summary>Answer</summary>

- `$@` - The target name (left side of the rule)
- `$<` - The first dependency (first item on right side)
- `$^` - All dependencies (all items on right side)

</details>

**Q1.3** Why do we use `.PHONY` targets?

<details>
<summary>Answer</summary>

`.PHONY` marks targets that don't represent actual files. Without it, if a file named `clean` existed, `make clean` would think the target is up-to-date and do nothing.

</details>

**Q1.4** What happens if you forget a header dependency in your Makefile?

<details>
<summary>Answer</summary>

If you change the header, files that include it won't be recompiled, leading to outdated object files and potentially undefined behavior or linker errors.

</details>

---

### Code Exercise 1.1: Complete the Makefile

Fill in the blanks to create a working Makefile:

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

SRC_DIR = src
BUILD_DIR = build

# TODO 1: Use wildcard to find all .cpp files in SRC_DIR
SRCS = _______________________________________

# TODO 2: Convert .cpp paths to .o paths in BUILD_DIR
# Example: src/main.cpp -> build/main.o
OBJS = _______________________________________

TARGET = compiler

all: $(TARGET)

# TODO 3: Write the linking rule
# Combine all object files into the executable
$(TARGET): $(OBJS)
	_______________________________________

# TODO 4: Write the pattern rule for compiling
# Convert any .cpp file to its corresponding .o file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	_______________________________________

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
```

<details>
<summary>Answer</summary>

```makefile
# TODO 1
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# TODO 2
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# TODO 3
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# TODO 4
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<
```

</details>

---

## Section 2: Lexical Analysis

### Theory Questions

**Q2.1** What is the difference between `peek()` and `advance()` in a lexer?

<details>
<summary>Answer</summary>

- `peek()` returns the current character without moving the position
- `advance()` returns the current character AND moves to the next position

</details>

**Q2.2** How do you distinguish keywords from identifiers?

<details>
<summary>Answer</summary>

Scan the identifier normally, then look it up in a keywords table. If found, return the keyword token type; otherwise return an identifier token.

</details>

**Q2.3** Why do we track line and column numbers in the lexer?

<details>
<summary>Answer</summary>

To provide meaningful error messages that help users locate problems in their source code.

</details>

**Q2.4** How would you handle nested comments like `/* outer /* inner */ still comment */`?

<details>
<summary>Answer</summary>

Use a depth counter. Increment on `/*`, decrement on `*/`. Only exit the comment when depth reaches 0.

</details>

---

### Code Exercise 2.1: Implement scan_identifier

Complete the function to scan identifiers and keywords:

```cpp
static const std::unordered_map<std::string, TokenType> keywords = {
    {"if",     TokenType::IF},
    {"else",   TokenType::ELSE},
    {"while",  TokenType::WHILE},
    {"return", TokenType::RETURN},
    {"var",    TokenType::VAR},
    {"func",   TokenType::FUNC},
};

Token Lexer::scan_identifier() {
    Position start(line_, column_);
    std::string text;

    // TODO 1: Collect all alphanumeric characters and underscores
    // Hint: Use std::isalnum() and check for '_'
    while (___________________________________) {
        _______________________________________
    }

    // TODO 2: Check if it's a keyword
    // If found in keywords map, return keyword token
    // Otherwise return ID token
    _______________________________________
    _______________________________________
    _______________________________________
    _______________________________________
}
```

<details>
<summary>Answer</summary>

```cpp
Token Lexer::scan_identifier() {
    Position start(line_, column_);
    std::string text;

    // TODO 1
    while (!at_end() && (std::isalnum(peek()) || peek() == '_')) {
        text += advance();
    }

    // TODO 2
    auto it = keywords.find(text);
    if (it != keywords.end()) {
        return Token(it->second, text, start);
    }
    return Token(TokenType::ID, text, start);
}
```

</details>

---

### Code Exercise 2.2: Implement scan_string with escape sequences

Complete the string scanning function:

```cpp
Token Lexer::scan_string() {
    Position start(line_, column_);
    advance();  // consume opening "

    std::string value;

    while (!at_end() && peek() != '"') {
        if (peek() == '\\') {
            // TODO 1: Handle escape sequences
            // Consume the backslash
            _______________________________________

            if (at_end()) {
                add_error("unterminated string");
                return Token(TokenType::ERROR, value, start);
            }

            // TODO 2: Process the escaped character
            // Handle: \n, \t, \\, \"
            char escaped = _______________________;
            switch (escaped) {
                case 'n':  ______________________ break;
                case 't':  ______________________ break;
                case '\\': ______________________ break;
                case '"':  ______________________ break;
                default:
                    add_error("unknown escape sequence");
                    value += escaped;
            }
        } else if (peek() == '\n') {
            // TODO 3: Handle error - newline in string
            _______________________________________
            break;
        } else {
            // TODO 4: Normal character
            _______________________________________
        }
    }

    // TODO 5: Check for unterminated string and consume closing quote
    _______________________________________
    _______________________________________
    _______________________________________

    return Token(TokenType::STRING_LIT, value, start);
}
```

<details>
<summary>Answer</summary>

```cpp
Token Lexer::scan_string() {
    Position start(line_, column_);
    advance();  // consume opening "

    std::string value;

    while (!at_end() && peek() != '"') {
        if (peek() == '\\') {
            // TODO 1
            advance();

            if (at_end()) {
                add_error("unterminated string");
                return Token(TokenType::ERROR, value, start);
            }

            // TODO 2
            char escaped = advance();
            switch (escaped) {
                case 'n':  value += '\n'; break;
                case 't':  value += '\t'; break;
                case '\\': value += '\\'; break;
                case '"':  value += '"';  break;
                default:
                    add_error("unknown escape sequence");
                    value += escaped;
            }
        } else if (peek() == '\n') {
            // TODO 3
            add_error("newline in string literal");
            break;
        } else {
            // TODO 4
            value += advance();
        }
    }

    // TODO 5
    if (at_end() || peek() != '"') {
        add_error("unterminated string");
        return Token(TokenType::ERROR, value, start);
    }
    advance();  // consume closing "

    return Token(TokenType::STRING_LIT, value, start);
}
```

</details>

---

### Code Exercise 2.3: Implement multi-character operators

Complete the operator handling in next_token:

```cpp
Token Lexer::next_token() {
    skip_whitespace_and_comments();

    if (at_end()) {
        return Token(TokenType::END_OF_FILE, "", Position(line_, column_));
    }

    Position start(line_, column_);
    char c = advance();

    switch (c) {
        // Simple single-character operators
        case '+': return Token(TokenType::PLUS, "+", start);
        case '-': return Token(TokenType::MINUS, "-", start);
        case '*': return Token(TokenType::STAR, "*", start);
        case '/': return Token(TokenType::SLASH, "/", start);

        // TODO 1: Handle '<' which could be '<', '<=', or '<>'
        case '<':
            _______________________________________
            _______________________________________
            _______________________________________
            _______________________________________
            _______________________________________

        // TODO 2: Handle '>' which could be '>' or '>='
        case '>':
            _______________________________________
            _______________________________________
            _______________________________________

        // TODO 3: Handle ':' which could be ':' or ':='
        case ':':
            _______________________________________
            _______________________________________
            _______________________________________

        // TODO 4: Handle '=' which could be '=' or '=='
        case '=':
            _______________________________________
            _______________________________________
            _______________________________________

        default:
            // ... handle other cases
    }
}
```

<details>
<summary>Answer</summary>

```cpp
        // TODO 1
        case '<':
            if (peek() == '=') { advance(); return Token(TokenType::LE, "<=", start); }
            if (peek() == '>') { advance(); return Token(TokenType::NEQ, "<>", start); }
            return Token(TokenType::LT, "<", start);

        // TODO 2
        case '>':
            if (peek() == '=') { advance(); return Token(TokenType::GE, ">=", start); }
            return Token(TokenType::GT, ">", start);

        // TODO 3
        case ':':
            if (peek() == '=') { advance(); return Token(TokenType::ASSIGN, ":=", start); }
            return Token(TokenType::COLON, ":", start);

        // TODO 4
        case '=':
            if (peek() == '=') { advance(); return Token(TokenType::EQ_EQ, "==", start); }
            return Token(TokenType::EQ, "=", start);
```

</details>

---

## Section 3: Parsing

### Theory Questions

**Q3.1** What is recursive descent parsing?

<details>
<summary>Answer</summary>

A top-down parsing technique where each grammar non-terminal is implemented as a function. The function body mirrors the grammar rule, and recursion naturally handles nested structures.

</details>

**Q3.2** Why do we need different precedence levels for operators?

<details>
<summary>Answer</summary>

To ensure operators bind correctly. For example, `1 + 2 * 3` should parse as `1 + (2 * 3)` because multiplication has higher precedence than addition.

</details>

**Q3.3** What's the difference between left and right associativity?

<details>
<summary>Answer</summary>

- **Left associative**: `a - b - c` = `(a - b) - c` (parsed left to right)
- **Right associative**: `a = b = c` = `a = (b = c)` (parsed right to left)

</details>

**Q3.4** What is the difference between `check()` and `match()` in a parser?

<details>
<summary>Answer</summary>

- `check(type)` returns true if the current token is of the given type, without consuming it
- `match(type)` returns true AND consumes the token if it matches; returns false otherwise

</details>

**Q3.5** What is the purpose of the `synchronize()` function in error recovery?

<details>
<summary>Answer</summary>

When an error is detected, `synchronize()` skips tokens until it finds a "safe" point (like the start of a new statement), allowing the parser to continue and find more errors instead of stopping at the first one.

</details>

---

### Code Exercise 3.1: Implement token handling methods

```cpp
class Parser {
    Lexer& lexer_;
    Token current_;

public:
    Parser(Lexer& lexer) : lexer_(lexer) {
        // TODO 1: Initialize current_ with the first token
        _______________________________________
    }

    Token peek() {
        // TODO 2: Return current token without consuming
        _______________________________________
    }

    Token advance() {
        // TODO 3: Save current token, get next, return saved
        _______________________________________
        _______________________________________
        _______________________________________
    }

    bool check(TokenType type) {
        // TODO 4: Check if current token matches type
        _______________________________________
    }

    bool match(TokenType type) {
        // TODO 5: If current matches type, advance and return true
        // Otherwise return false
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
    }

    void expect(TokenType type, const char* msg) {
        // TODO 6: If current matches, advance. Otherwise error.
        _______________________________________
        _______________________________________
        _______________________________________
    }
};
```

<details>
<summary>Answer</summary>

```cpp
    Parser(Lexer& lexer) : lexer_(lexer) {
        // TODO 1
        current_ = lexer_.next_token();
    }

    Token peek() {
        // TODO 2
        return current_;
    }

    Token advance() {
        // TODO 3
        Token prev = current_;
        current_ = lexer_.next_token();
        return prev;
    }

    bool check(TokenType type) {
        // TODO 4
        return current_.type == type;
    }

    bool match(TokenType type) {
        // TODO 5
        if (check(type)) {
            advance();
            return true;
        }
        return false;
    }

    void expect(TokenType type, const char* msg) {
        // TODO 6
        if (!match(type)) {
            error(msg);
        }
    }
```

</details>

---

### Code Exercise 3.2: Implement operator precedence parsing

Implement the additive and multiplicative expression parsers:

```cpp
// Grammar:
//   additive    → multiplicative (('+' | '-') multiplicative)*
//   multiplicative → unary (('*' | '/') unary)*

ExpPtr Parser::parse_additive() {
    // TODO 1: Start by parsing a multiplicative expression
    ExpPtr left = _______________________________________

    // TODO 2: While we see + or -, parse more terms
    while (true) {
        Position pos = current_.pos;
        Op op;

        // TODO 3: Check for + or - and set op accordingly
        if (match(TokenType::PLUS)) {
            _______________________________________
        } else if (match(TokenType::MINUS)) {
            _______________________________________
        } else {
            _______________________________________  // No more operators
        }

        // TODO 4: Parse the right operand (multiplicative)
        ExpPtr right = _______________________________________

        // TODO 5: Build the OpExp node (left associative)
        left = _______________________________________
    }

    return left;
}

ExpPtr Parser::parse_multiplicative() {
    // TODO 6: Start by parsing a unary expression
    ExpPtr left = _______________________________________

    // TODO 7: While we see * or /, parse more factors
    while (true) {
        Position pos = current_.pos;
        Op op;

        // TODO 8: Check for * or / and set op accordingly
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________

        // TODO 9: Parse the right operand
        ExpPtr right = _______________________________________

        // TODO 10: Build the OpExp node
        left = _______________________________________
    }

    return left;
}
```

<details>
<summary>Answer</summary>

```cpp
ExpPtr Parser::parse_additive() {
    // TODO 1
    ExpPtr left = parse_multiplicative();

    // TODO 2
    while (true) {
        Position pos = current_.pos;
        Op op;

        // TODO 3
        if (match(TokenType::PLUS)) {
            op = Op::PLUS;
        } else if (match(TokenType::MINUS)) {
            op = Op::MINUS;
        } else {
            break;
        }

        // TODO 4
        ExpPtr right = parse_multiplicative();

        // TODO 5
        left = std::make_unique<OpExp>(std::move(left), op, std::move(right), pos);
    }

    return left;
}

ExpPtr Parser::parse_multiplicative() {
    // TODO 6
    ExpPtr left = parse_unary();

    // TODO 7
    while (true) {
        Position pos = current_.pos;
        Op op;

        // TODO 8
        if (match(TokenType::STAR)) {
            op = Op::TIMES;
        } else if (match(TokenType::SLASH)) {
            op = Op::DIVIDE;
        } else {
            break;
        }

        // TODO 9
        ExpPtr right = parse_unary();

        // TODO 10
        left = std::make_unique<OpExp>(std::move(left), op, std::move(right), pos);
    }

    return left;
}
```

</details>

---

### Code Exercise 3.3: Implement if-else parsing

```cpp
// Grammar: if_exp → 'if' expression 'then' expression ('else' expression)?

ExpPtr Parser::parse_if() {
    Position pos = current_.pos;

    // TODO 1: Consume the 'if' keyword
    _______________________________________

    // TODO 2: Parse the condition expression
    ExpPtr condition = _______________________________________

    // TODO 3: Expect and consume 'then'
    _______________________________________

    // TODO 4: Parse the then-branch expression
    ExpPtr then_branch = _______________________________________

    // TODO 5: Check for optional 'else' and parse else-branch if present
    ExpPtr else_branch = nullptr;
    _______________________________________
    _______________________________________
    _______________________________________

    // TODO 6: Construct and return the IfExp
    return _______________________________________
}
```

<details>
<summary>Answer</summary>

```cpp
ExpPtr Parser::parse_if() {
    Position pos = current_.pos;

    // TODO 1
    expect(TokenType::IF, "expected 'if'");

    // TODO 2
    ExpPtr condition = parse_expression();

    // TODO 3
    expect(TokenType::THEN, "expected 'then'");

    // TODO 4
    ExpPtr then_branch = parse_expression();

    // TODO 5
    ExpPtr else_branch = nullptr;
    if (match(TokenType::ELSE)) {
        else_branch = parse_expression();
    }

    // TODO 6
    return std::make_unique<IfExp>(
        std::move(condition),
        std::move(then_branch),
        std::move(else_branch),
        pos
    );
}
```

</details>

---

### Code Exercise 3.4: Implement while loop parsing

```cpp
// Grammar: while_exp → 'while' expression 'do' expression

ExpPtr Parser::parse_while() {
    // TODO: Implement the while expression parser
    // 1. Save position
    // 2. Consume 'while'
    // 3. Parse condition
    // 4. Expect 'do'
    // 5. Parse body
    // 6. Return WhileExp

    _______________________________________
    _______________________________________
    _______________________________________
    _______________________________________
    _______________________________________
    _______________________________________
}
```

<details>
<summary>Answer</summary>

```cpp
ExpPtr Parser::parse_while() {
    Position pos = current_.pos;
    expect(TokenType::WHILE, "expected 'while'");
    ExpPtr condition = parse_expression();
    expect(TokenType::DO, "expected 'do'");
    ExpPtr body = parse_expression();
    return std::make_unique<WhileExp>(std::move(condition), std::move(body), pos);
}
```

</details>

---

### Code Exercise 3.5: Implement function call parsing

```cpp
// Grammar: call → ID '(' (expression (',' expression)*)? ')'

ExpPtr Parser::parse_call(const std::string& name, Position pos) {
    // Assume '(' has already been consumed

    std::vector<ExpPtr> args;

    // TODO 1: Check if there are any arguments (not immediately ')' )
    if (!check(TokenType::RPAREN)) {
        // TODO 2: Parse first argument
        _______________________________________

        // TODO 3: While there's a comma, parse more arguments
        while (match(TokenType::COMMA)) {
            _______________________________________
        }
    }

    // TODO 4: Expect closing ')'
    _______________________________________

    // TODO 5: Return CallExp
    return _______________________________________
}
```

<details>
<summary>Answer</summary>

```cpp
ExpPtr Parser::parse_call(const std::string& name, Position pos) {
    std::vector<ExpPtr> args;

    // TODO 1 & 2
    if (!check(TokenType::RPAREN)) {
        args.push_back(parse_expression());

        // TODO 3
        while (match(TokenType::COMMA)) {
            args.push_back(parse_expression());
        }
    }

    // TODO 4
    expect(TokenType::RPAREN, "expected ')'");

    // TODO 5
    return std::make_unique<CallExp>(name, std::move(args), pos);
}
```

</details>

---

### Code Exercise 3.6: Implement let expression parsing

```cpp
// Grammar:
//   let_exp → 'let' declaration* 'in' expression* 'end'
//   declaration → var_dec | type_dec | function_dec

ExpPtr Parser::parse_let() {
    Position pos = current_.pos;

    // TODO 1: Consume 'let'
    _______________________________________

    // TODO 2: Parse declarations until we see 'in'
    std::vector<DecPtr> decs;
    while (_______________________________________) {
        DecPtr dec = parse_declaration();
        if (dec) {
            _______________________________________
        }
    }

    // TODO 3: Expect 'in'
    _______________________________________

    // TODO 4: Parse body expressions (separated by semicolons)
    std::vector<ExpPtr> body;
    if (!check(TokenType::END)) {
        _______________________________________
        while (match(TokenType::SEMI)) {
            if (check(TokenType::END)) break;
            _______________________________________
        }
    }

    // TODO 5: Expect 'end'
    _______________________________________

    // TODO 6: Return LetExp
    return _______________________________________
}
```

<details>
<summary>Answer</summary>

```cpp
ExpPtr Parser::parse_let() {
    Position pos = current_.pos;

    // TODO 1
    expect(TokenType::LET, "expected 'let'");

    // TODO 2
    std::vector<DecPtr> decs;
    while (!check(TokenType::IN) && !check(TokenType::END_OF_FILE)) {
        DecPtr dec = parse_declaration();
        if (dec) {
            decs.push_back(std::move(dec));
        }
    }

    // TODO 3
    expect(TokenType::IN, "expected 'in'");

    // TODO 4
    std::vector<ExpPtr> body;
    if (!check(TokenType::END)) {
        body.push_back(parse_expression());
        while (match(TokenType::SEMI)) {
            if (check(TokenType::END)) break;
            body.push_back(parse_expression());
        }
    }

    // TODO 5
    expect(TokenType::END, "expected 'end'");

    // TODO 6
    return std::make_unique<LetExp>(std::move(decs), std::move(body), pos);
}
```

</details>

---

## Section 4: AST Design

### Theory Questions

**Q4.1** Why do we use `std::unique_ptr` for AST nodes?

<details>
<summary>Answer</summary>

- Automatic memory management (no manual delete)
- Clear ownership semantics (one owner)
- Move semantics for efficient transfers
- Prevents memory leaks

</details>

**Q4.2** Why include Position in AST nodes?

<details>
<summary>Answer</summary>

To provide accurate error messages in later compiler phases (type checking, code generation) that refer back to the original source location.

</details>

**Q4.3** What's the advantage of using an enum `kind` field versus virtual methods?

<details>
<summary>Answer</summary>

- **Enum + switch**: Explicit control, easy to add new operations (visitors), all cases visible
- **Virtual methods**: Easy to add new node types, encapsulates behavior

The enum approach is common in compilers because you typically add operations (analysis passes) more often than node types.

</details>

---

### Code Exercise 4.1: Define AST node types

```cpp
// Define the AST nodes for a simple expression language:
// - Integer literals
// - Binary operations (+, -, *, /)
// - Variable references
// - Let expressions: let x = e1 in e2

enum class ExpKind {
    // TODO 1: List all expression kinds
    _______________________________________
};

struct Exp {
    ExpKind kind;
    Position pos;

    Exp(ExpKind k, Position p) : kind(k), pos(p) {}
    virtual ~Exp() = default;
};

// TODO 2: Define IntExp
struct IntExp : Exp {
    _______________________________________

    IntExp(_______________________, Position p)
        : Exp(ExpKind::INT, p), _______________________ {}
};

// TODO 3: Define BinOpExp (binary operation)
enum class Op { PLUS, MINUS, TIMES, DIVIDE };

struct BinOpExp : Exp {
    _______________________________________
    _______________________________________
    _______________________________________

    BinOpExp(_______________________________________,
             Position p)
        : Exp(ExpKind::BINOP, p),
          _______________________________________,
          _______________________________________,
          _______________________________________ {}
};

// TODO 4: Define VarExp (variable reference)
struct VarExp : Exp {
    _______________________________________

    VarExp(_______________________, Position p)
        : Exp(ExpKind::VAR, p), _______________________ {}
};

// TODO 5: Define LetExp
// let name = init in body
struct LetExp : Exp {
    _______________________________________
    _______________________________________
    _______________________________________

    LetExp(_______________________________________,
           Position p)
        : Exp(ExpKind::LET, p),
          _______________________________________,
          _______________________________________,
          _______________________________________ {}
};
```

<details>
<summary>Answer</summary>

```cpp
enum class ExpKind {
    // TODO 1
    INT, BINOP, VAR, LET
};

// TODO 2
struct IntExp : Exp {
    int value;

    IntExp(int v, Position p)
        : Exp(ExpKind::INT, p), value(v) {}
};

// TODO 3
struct BinOpExp : Exp {
    std::unique_ptr<Exp> left;
    Op op;
    std::unique_ptr<Exp> right;

    BinOpExp(std::unique_ptr<Exp> l, Op o, std::unique_ptr<Exp> r,
             Position p)
        : Exp(ExpKind::BINOP, p),
          left(std::move(l)),
          op(o),
          right(std::move(r)) {}
};

// TODO 4
struct VarExp : Exp {
    std::string name;

    VarExp(const std::string& n, Position p)
        : Exp(ExpKind::VAR, p), name(n) {}
};

// TODO 5
struct LetExp : Exp {
    std::string name;
    std::unique_ptr<Exp> init;
    std::unique_ptr<Exp> body;

    LetExp(const std::string& n, std::unique_ptr<Exp> i, std::unique_ptr<Exp> b,
           Position p)
        : Exp(ExpKind::LET, p),
          name(n),
          init(std::move(i)),
          body(std::move(b)) {}
};
```

</details>

---

## Section 5: Integration

### Code Exercise 5.1: Complete a mini-language parser

Implement a parser for this simple grammar:

```
program    → expression EOF
expression → term (('+' | '-') term)*
term       → factor (('*' | '/') factor)*
factor     → NUMBER | ID | '(' expression ')'
```

```cpp
class MiniParser {
    MiniLexer& lexer_;
    Token current_;

public:
    MiniParser(MiniLexer& lexer) : lexer_(lexer) {
        current_ = lexer_.next_token();
    }

    ExpPtr parse() {
        ExpPtr exp = parse_expression();
        expect(TokenType::END_OF_FILE, "expected end of input");
        return exp;
    }

private:
    // Token methods (assume these are implemented)
    Token peek() { return current_; }
    Token advance() { Token t = current_; current_ = lexer_.next_token(); return t; }
    bool check(TokenType t) { return current_.type == t; }
    bool match(TokenType t) { if (check(t)) { advance(); return true; } return false; }
    void expect(TokenType t, const char* msg) { if (!match(t)) error(msg); }
    void error(const char* msg) { throw std::runtime_error(msg); }

    // TODO: Implement these three parsing methods

    ExpPtr parse_expression() {
        // expression → term (('+' | '-') term)*
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
    }

    ExpPtr parse_term() {
        // term → factor (('*' | '/') factor)*
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
    }

    ExpPtr parse_factor() {
        // factor → NUMBER | ID | '(' expression ')'
        Position pos = current_.pos;

        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________
        _______________________________________

        error("expected expression");
        return nullptr;
    }
};
```

<details>
<summary>Answer</summary>

```cpp
    ExpPtr parse_expression() {
        // expression → term (('+' | '-') term)*
        ExpPtr left = parse_term();

        while (true) {
            Position pos = current_.pos;
            Op op;
            if (match(TokenType::PLUS)) {
                op = Op::PLUS;
            } else if (match(TokenType::MINUS)) {
                op = Op::MINUS;
            } else {
                break;
            }
            ExpPtr right = parse_term();
            left = std::make_unique<BinOpExp>(std::move(left), op, std::move(right), pos);
        }

        return left;
    }

    ExpPtr parse_term() {
        // term → factor (('*' | '/') factor)*
        ExpPtr left = parse_factor();

        while (true) {
            Position pos = current_.pos;
            Op op;
            if (match(TokenType::STAR)) {
                op = Op::TIMES;
            } else if (match(TokenType::SLASH)) {
                op = Op::DIVIDE;
            } else {
                break;
            }
            ExpPtr right = parse_factor();
            left = std::make_unique<BinOpExp>(std::move(left), op, std::move(right), pos);
        }

        return left;
    }

    ExpPtr parse_factor() {
        // factor → NUMBER | ID | '(' expression ')'
        Position pos = current_.pos;

        if (check(TokenType::INT_LIT)) {
            Token tok = advance();
            return std::make_unique<IntExp>(tok.int_value, pos);
        }

        if (check(TokenType::ID)) {
            Token tok = advance();
            return std::make_unique<VarExp>(tok.text, pos);
        }

        if (match(TokenType::LPAREN)) {
            ExpPtr exp = parse_expression();
            expect(TokenType::RPAREN, "expected ')'");
            return exp;
        }

        error("expected expression");
        return nullptr;
    }
```

</details>

---

## Final Challenge

### Design Your Own Language

Design and implement a parser for a small language of your choice. It should include:

1. **At least 3 different expression types** (e.g., literals, operations, function calls)
2. **At least 2 different statement types** (e.g., variable declaration, if statement)
3. **Proper operator precedence** (at least 2 levels)
4. **Error messages with line/column information**

Steps:
1. Write the grammar for your language
2. Design the token types
3. Design the AST node types
4. Implement the lexer
5. Implement the parser
6. Write a Makefile
7. Test with example programs

---

## Grading Rubric

| Section | Points |
|---------|--------|
| Build Systems (Section 1) | 15 |
| Lexer (Section 2) | 25 |
| Parser (Section 3) | 35 |
| AST Design (Section 4) | 15 |
| Integration (Section 5) | 10 |
| **Total** | **100** |

---

## Answer Key Summary

All detailed answers are provided in the `<details>` sections above. To check your work:

1. Compare your solution with the provided answer
2. Test your code with sample inputs
3. Verify error handling works correctly
4. Check that the AST structure is correct

Good luck!
