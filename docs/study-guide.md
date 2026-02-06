# Building Compilers: Lexers, Parsers, and Build Systems

A comprehensive guide for implementing your own programming language parser.

---

## Table of Contents

1. [Part 1: C++ Build Systems](#part-1-c-build-systems)
2. [Part 2: Lexical Analysis (Lexer)](#part-2-lexical-analysis-lexer)
3. [Part 3: Syntax Analysis (Parser)](#part-3-syntax-analysis-parser)
4. [Part 4: Abstract Syntax Trees (AST)](#part-4-abstract-syntax-trees-ast)
5. [Part 5: Putting It All Together](#part-5-putting-it-all-together)

---

## Part 1: C++ Build Systems

### 1.1 Why Build Systems?

When your project has multiple source files, you need to:
- Compile each `.cpp` file into an object file (`.o`)
- Link all object files into an executable
- Recompile only changed files (incremental builds)
- Manage dependencies between files

### 1.2 The Compilation Process

```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│  source.cpp │────▶│  source.o   │────▶│ executable  │
│  (source)   │     │  (object)   │     │  (binary)   │
└─────────────┘     └─────────────┘     └─────────────┘
     Compile             Link
```

**Step 1: Compile** - Convert source to object file:
```bash
g++ -c -o source.o source.cpp
```

**Step 2: Link** - Combine object files into executable:
```bash
g++ -o myprogram main.o lexer.o parser.o
```

### 1.3 Understanding Makefiles

A Makefile defines **targets**, **dependencies**, and **recipes**.

```makefile
target: dependencies
	recipe (must be TAB-indented)
```

#### Basic Makefile Structure

```makefile
# Variables
CXX = g++                           # Compiler
CXXFLAGS = -std=c++17 -Wall -g      # Compiler flags

# Directories
SRC_DIR = src
BUILD_DIR = build

# File lists
SRCS = $(wildcard $(SRC_DIR)/*.cpp)                    # All .cpp files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)       # Convert to .o paths
TARGET = mycompiler

# Default target
all: $(TARGET)

# Link: combine object files into executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile: convert .cpp to .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Phony targets (not actual files)
.PHONY: all clean
```

#### Key Makefile Concepts

| Symbol | Meaning | Example |
|--------|---------|---------|
| `$@` | Target name | In rule for `foo.o`, `$@` = `foo.o` |
| `$<` | First dependency | First file in dependency list |
| `$^` | All dependencies | All files in dependency list |
| `%` | Pattern wildcard | `%.o: %.cpp` matches any `.o`/`.cpp` pair |
| `|` | Order-only dependency | Must exist, but timestamp ignored |

#### Dependency Management

Headers create implicit dependencies. If `parser.cpp` includes `lexer.hpp`, changing `lexer.hpp` should recompile `parser.cpp`.

**Manual approach:**
```makefile
$(BUILD_DIR)/parser.o: $(SRC_DIR)/parser.hpp $(SRC_DIR)/lexer.hpp
```

**Automatic approach** (let compiler generate dependencies):
```makefile
CXXFLAGS += -MMD -MP
-include $(OBJS:.o=.d)
```

### 1.4 Common Compiler Flags

| Flag | Purpose |
|------|---------|
| `-std=c++17` | Use C++17 standard |
| `-Wall` | Enable common warnings |
| `-Wextra` | Enable extra warnings |
| `-g` | Include debug symbols |
| `-O2` | Optimization level 2 |
| `-c` | Compile only (don't link) |
| `-o <file>` | Output file name |
| `-I<dir>` | Add include directory |

### 1.5 Project Structure Best Practices

```
mycompiler/
├── Makefile
├── src/
│   ├── main.cpp
│   ├── token.hpp
│   ├── token.cpp
│   ├── lexer.hpp
│   ├── lexer.cpp
│   ├── ast.hpp
│   ├── parser.hpp
│   └── parser.cpp
├── build/           # Generated object files
├── tests/
│   └── test1.lang
└── docs/
```

---

## Part 2: Lexical Analysis (Lexer)

### 2.1 What is a Lexer?

A **lexer** (or **scanner** or **tokenizer**) converts source code (a stream of characters) into a stream of **tokens**.

```
Source: "let x := 10 + 20"

Tokens: [LET] [ID:"x"] [ASSIGN] [INT:10] [PLUS] [INT:20] [EOF]
```

### 2.2 Token Structure

```cpp
// Position tracks where tokens appear (for error messages)
struct Position {
    int line;
    int column;

    Position(int l = 1, int c = 1) : line(l), column(c) {}
};

// TokenType enumerates all possible token kinds
enum class TokenType {
    // Literals
    INT_LIT,        // 123
    STRING_LIT,     // "hello"

    // Identifiers
    ID,             // foo, bar, myVar

    // Keywords (reserved words)
    LET, IF, ELSE, WHILE, FOR, FUNCTION, RETURN,

    // Operators
    PLUS, MINUS, STAR, SLASH,   // + - * /
    EQ, NEQ, LT, LE, GT, GE,    // = <> < <= > >=
    ASSIGN,                      // :=

    // Punctuation
    LPAREN, RPAREN,  // ( )
    LBRACE, RBRACE,  // { }
    LBRACK, RBRACK,  // [ ]
    COMMA, SEMI, COLON, DOT,

    // Special
    END_OF_FILE,
    ERROR,
};

// Token combines type, text, position, and optional value
struct Token {
    TokenType type;
    std::string text;
    Position pos;
    int int_value;      // For INT_LIT tokens

    Token(TokenType t, const std::string& txt, Position p)
        : type(t), text(txt), pos(p), int_value(0) {}
};
```

### 2.3 Lexer Implementation Pattern

```cpp
class Lexer {
public:
    explicit Lexer(const std::string& source);

    Token next_token();     // Get next token, advance position
    Token peek_token();     // Get next token, don't advance

private:
    std::string source_;    // Source code
    size_t pos_;            // Current position in source
    int line_, column_;     // Current line and column

    // Character-level operations
    char peek() const;      // Look at current char
    char peek_next() const; // Look at next char
    char advance();         // Consume and return current char
    bool at_end() const;    // Are we at end of source?

    // Scanning methods
    void skip_whitespace_and_comments();
    Token scan_identifier();
    Token scan_number();
    Token scan_string();
};
```

### 2.4 Core Lexer Methods

#### Character Operations

```cpp
char Lexer::peek() const {
    if (pos_ >= source_.size()) return '\0';
    return source_[pos_];
}

char Lexer::peek_next() const {
    if (pos_ + 1 >= source_.size()) return '\0';
    return source_[pos_ + 1];
}

char Lexer::advance() {
    char c = source_[pos_++];
    if (c == '\n') {
        line_++;
        column_ = 1;
    } else {
        column_++;
    }
    return c;
}

bool Lexer::at_end() const {
    return pos_ >= source_.size();
}
```

#### Scanning Identifiers and Keywords

```cpp
// Keywords map - check if identifier is actually a keyword
static const std::unordered_map<std::string, TokenType> keywords = {
    {"let",      TokenType::LET},
    {"if",       TokenType::IF},
    {"else",     TokenType::ELSE},
    {"while",    TokenType::WHILE},
    {"function", TokenType::FUNCTION},
    // ... add more keywords
};

Token Lexer::scan_identifier() {
    Position start(line_, column_);
    std::string text;

    // Identifiers: [a-zA-Z_][a-zA-Z0-9_]*
    while (!at_end() && (std::isalnum(peek()) || peek() == '_')) {
        text += advance();
    }

    // Check if it's a keyword
    auto it = keywords.find(text);
    if (it != keywords.end()) {
        return Token(it->second, text, start);
    }
    return Token(TokenType::ID, text, start);
}
```

#### Scanning Numbers

```cpp
Token Lexer::scan_number() {
    Position start(line_, column_);
    std::string text;

    while (!at_end() && std::isdigit(peek())) {
        text += advance();
    }

    Token tok(TokenType::INT_LIT, text, start);
    tok.int_value = std::stoi(text);
    return tok;
}
```

#### Scanning Strings

```cpp
Token Lexer::scan_string() {
    Position start(line_, column_);
    advance();  // consume opening quote

    std::string value;
    while (!at_end() && peek() != '"') {
        if (peek() == '\\') {
            advance();  // consume backslash
            switch (peek()) {
                case 'n':  value += '\n'; break;
                case 't':  value += '\t'; break;
                case '\\': value += '\\'; break;
                case '"':  value += '"';  break;
                default:   /* error: unknown escape */ break;
            }
            advance();
        } else {
            value += advance();
        }
    }

    if (at_end()) {
        // Error: unterminated string
    }
    advance();  // consume closing quote

    return Token(TokenType::STRING_LIT, value, start);
}
```

#### Main Token Dispatch

```cpp
Token Lexer::next_token() {
    skip_whitespace_and_comments();

    if (at_end()) {
        return Token(TokenType::END_OF_FILE, "", Position(line_, column_));
    }

    Position start(line_, column_);
    char c = peek();

    // Identifiers and keywords
    if (std::isalpha(c) || c == '_') {
        return scan_identifier();
    }

    // Numbers
    if (std::isdigit(c)) {
        return scan_number();
    }

    // Strings
    if (c == '"') {
        return scan_string();
    }

    // Single and multi-character operators
    advance();  // consume the character

    switch (c) {
        case '+': return Token(TokenType::PLUS, "+", start);
        case '-': return Token(TokenType::MINUS, "-", start);
        case '*': return Token(TokenType::STAR, "*", start);
        case '/': return Token(TokenType::SLASH, "/", start);

        // Multi-character: check next character
        case '<':
            if (peek() == '=') { advance(); return Token(TokenType::LE, "<=", start); }
            if (peek() == '>') { advance(); return Token(TokenType::NEQ, "<>", start); }
            return Token(TokenType::LT, "<", start);

        case ':':
            if (peek() == '=') { advance(); return Token(TokenType::ASSIGN, ":=", start); }
            return Token(TokenType::COLON, ":", start);

        // ... handle more operators and punctuation

        default:
            return Token(TokenType::ERROR, std::string(1, c), start);
    }
}
```

### 2.5 Handling Comments

#### Single-line Comments (// ...)

```cpp
void Lexer::skip_line_comment() {
    while (!at_end() && peek() != '\n') {
        advance();
    }
}
```

#### Multi-line Comments (/* ... */)

```cpp
void Lexer::skip_block_comment() {
    advance(); // consume /
    advance(); // consume *

    while (!at_end()) {
        if (peek() == '*' && peek_next() == '/') {
            advance(); // consume *
            advance(); // consume /
            return;
        }
        advance();
    }
    // Error: unterminated comment
}
```

#### Nested Comments (Tiger-style)

```cpp
bool Lexer::skip_nested_comment() {
    if (peek() != '/' || peek_next() != '*') return false;

    advance(); advance();  // consume /*

    int depth = 1;
    while (!at_end() && depth > 0) {
        if (peek() == '/' && peek_next() == '*') {
            advance(); advance();
            depth++;
        } else if (peek() == '*' && peek_next() == '/') {
            advance(); advance();
            depth--;
        } else {
            advance();
        }
    }

    return depth == 0;  // false if unterminated
}
```

---

## Part 3: Syntax Analysis (Parser)

### 3.1 What is a Parser?

A **parser** takes a stream of tokens and builds a structured representation (usually an AST) according to the language's grammar.

```
Tokens: [LET] [ID:x] [ASSIGN] [INT:10] [PLUS] [INT:20] [IN] [ID:x] [END]

AST:
  LetExp
    ├── VarDec: x = OpExp(+)
    │              ├── IntExp(10)
    │              └── IntExp(20)
    └── body: VarExp(x)
```

### 3.2 Context-Free Grammars

Languages are defined by **grammars**. A grammar has:
- **Terminals**: Tokens (the leaves)
- **Non-terminals**: Syntactic categories (the internal nodes)
- **Productions**: Rules defining how non-terminals expand

#### Example Grammar (simplified expression language)

```
program     → expression EOF

expression  → term (('+' | '-') term)*
term        → factor (('*' | '/') factor)*
factor      → NUMBER | '(' expression ')'
```

This grammar defines:
- A program is an expression followed by end-of-file
- An expression is terms separated by + or -
- A term is factors separated by * or /
- A factor is a number or a parenthesized expression

### 3.3 Recursive Descent Parsing

**Recursive descent** is a top-down parsing technique where:
- Each non-terminal in the grammar becomes a function
- The function body mirrors the grammar rule
- Recursion handles nested structures

#### Parser Structure

```cpp
class Parser {
public:
    explicit Parser(Lexer& lexer);
    std::unique_ptr<Program> parse();

private:
    Lexer& lexer_;
    Token current_;          // Current token

    // Token operations
    Token peek();            // Look at current token
    Token advance();         // Consume and return current, get next
    bool check(TokenType t); // Is current token of type t?
    bool match(TokenType t); // If current is t, advance and return true
    void expect(TokenType t, const char* msg);  // Require token or error

    // Non-terminal parsing methods (one per grammar rule)
    ExpPtr parse_expression();
    ExpPtr parse_term();
    ExpPtr parse_factor();
    // ... etc
};
```

### 3.4 Token Handling Methods

```cpp
Token Parser::peek() {
    return current_;
}

Token Parser::advance() {
    Token prev = current_;
    current_ = lexer_.next_token();
    return prev;
}

bool Parser::check(TokenType type) {
    return current_.type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

void Parser::expect(TokenType type, const char* msg) {
    if (!match(type)) {
        error(msg);
    }
}
```

### 3.5 Operator Precedence

Different operators have different **precedence** (binding strength) and **associativity** (grouping direction).

```
Expression: 1 + 2 * 3 - 4

With correct precedence (* before +/-):
  ((1 + (2 * 3)) - 4) = (1 + 6) - 4 = 3
```

#### Precedence Table (low to high)

| Level | Operators | Associativity |
|-------|-----------|---------------|
| 1 | `:=` (assignment) | Right |
| 2 | `\|` (or) | Left |
| 3 | `&` (and) | Left |
| 4 | `= <> < <= > >=` (comparison) | Left |
| 5 | `+ -` (additive) | Left |
| 6 | `* /` (multiplicative) | Left |
| 7 | `-` (unary minus) | Right |

#### Implementing Precedence with Separate Functions

```cpp
// Each precedence level gets its own function
// Lower precedence = called first (outer)
// Higher precedence = called later (inner)

ExpPtr Parser::parse_expression() {
    return parse_assignment();  // Lowest precedence
}

ExpPtr Parser::parse_assignment() {
    ExpPtr left = parse_or();

    if (match(TokenType::ASSIGN)) {
        ExpPtr right = parse_assignment();  // Right associative
        return make_assign(left, right);
    }
    return left;
}

ExpPtr Parser::parse_or() {
    ExpPtr left = parse_and();

    while (match(TokenType::OR)) {
        ExpPtr right = parse_and();
        left = make_or(left, right);  // Left associative
    }
    return left;
}

ExpPtr Parser::parse_and() {
    ExpPtr left = parse_comparison();

    while (match(TokenType::AND)) {
        ExpPtr right = parse_comparison();
        left = make_and(left, right);
    }
    return left;
}

ExpPtr Parser::parse_comparison() {
    ExpPtr left = parse_additive();

    while (true) {
        Op op;
        if (match(TokenType::EQ))       op = Op::EQ;
        else if (match(TokenType::NEQ)) op = Op::NEQ;
        else if (match(TokenType::LT))  op = Op::LT;
        else if (match(TokenType::LE))  op = Op::LE;
        else if (match(TokenType::GT))  op = Op::GT;
        else if (match(TokenType::GE))  op = Op::GE;
        else break;

        ExpPtr right = parse_additive();
        left = std::make_unique<OpExp>(std::move(left), op, std::move(right));
    }
    return left;
}

ExpPtr Parser::parse_additive() {
    ExpPtr left = parse_multiplicative();

    while (true) {
        Op op;
        if (match(TokenType::PLUS))       op = Op::PLUS;
        else if (match(TokenType::MINUS)) op = Op::MINUS;
        else break;

        ExpPtr right = parse_multiplicative();
        left = std::make_unique<OpExp>(std::move(left), op, std::move(right));
    }
    return left;
}

ExpPtr Parser::parse_multiplicative() {
    ExpPtr left = parse_unary();

    while (true) {
        Op op;
        if (match(TokenType::STAR))       op = Op::TIMES;
        else if (match(TokenType::SLASH)) op = Op::DIVIDE;
        else break;

        ExpPtr right = parse_unary();
        left = std::make_unique<OpExp>(std::move(left), op, std::move(right));
    }
    return left;
}

ExpPtr Parser::parse_unary() {
    if (match(TokenType::MINUS)) {
        ExpPtr operand = parse_unary();  // Right associative
        // Represent -x as (0 - x)
        return std::make_unique<OpExp>(
            std::make_unique<IntExp>(0),
            Op::MINUS,
            std::move(operand)
        );
    }
    return parse_primary();
}
```

### 3.6 Parsing Primary Expressions

Primary expressions are the "atoms" - they don't contain binary operators at the top level.

```cpp
ExpPtr Parser::parse_primary() {
    Position pos = current_.pos;

    // nil literal
    if (match(TokenType::NIL)) {
        return std::make_unique<NilExp>(pos);
    }

    // Integer literal
    if (check(TokenType::INT_LIT)) {
        Token tok = advance();
        return std::make_unique<IntExp>(tok.int_value, pos);
    }

    // String literal
    if (check(TokenType::STRING_LIT)) {
        Token tok = advance();
        return std::make_unique<StringExp>(tok.text, pos);
    }

    // Parenthesized expression
    if (match(TokenType::LPAREN)) {
        ExpPtr exp = parse_expression();
        expect(TokenType::RPAREN, "expected ')'");
        return exp;
    }

    // If expression
    if (check(TokenType::IF)) {
        return parse_if();
    }

    // While expression
    if (check(TokenType::WHILE)) {
        return parse_while();
    }

    // Let expression
    if (check(TokenType::LET)) {
        return parse_let();
    }

    // Identifier (variable, function call, etc.)
    if (check(TokenType::ID)) {
        return parse_identifier_expr();
    }

    error("expected expression");
    return std::make_unique<NilExp>(pos);  // Error recovery
}
```

### 3.7 Parsing Control Flow

#### If Expression

```
Grammar: if_exp → 'if' expression 'then' expression ('else' expression)?
```

```cpp
ExpPtr Parser::parse_if() {
    Position pos = current_.pos;
    expect(TokenType::IF, "expected 'if'");

    ExpPtr condition = parse_expression();
    expect(TokenType::THEN, "expected 'then'");
    ExpPtr then_branch = parse_expression();

    ExpPtr else_branch = nullptr;
    if (match(TokenType::ELSE)) {
        else_branch = parse_expression();
    }

    return std::make_unique<IfExp>(
        std::move(condition),
        std::move(then_branch),
        std::move(else_branch),
        pos
    );
}
```

#### While Expression

```
Grammar: while_exp → 'while' expression 'do' expression
```

```cpp
ExpPtr Parser::parse_while() {
    Position pos = current_.pos;
    expect(TokenType::WHILE, "expected 'while'");

    ExpPtr condition = parse_expression();
    expect(TokenType::DO, "expected 'do'");
    ExpPtr body = parse_expression();

    return std::make_unique<WhileExp>(
        std::move(condition),
        std::move(body),
        pos
    );
}
```

#### For Expression

```
Grammar: for_exp → 'for' ID ':=' expression 'to' expression 'do' expression
```

```cpp
ExpPtr Parser::parse_for() {
    Position pos = current_.pos;
    expect(TokenType::FOR, "expected 'for'");

    if (!check(TokenType::ID)) {
        error("expected identifier");
    }
    std::string var = advance().text;

    expect(TokenType::ASSIGN, "expected ':='");
    ExpPtr lo = parse_expression();
    expect(TokenType::TO, "expected 'to'");
    ExpPtr hi = parse_expression();
    expect(TokenType::DO, "expected 'do'");
    ExpPtr body = parse_expression();

    return std::make_unique<ForExp>(var, std::move(lo), std::move(hi), std::move(body), pos);
}
```

### 3.8 Parsing Let Expressions and Declarations

```
Grammar:
  let_exp → 'let' declaration* 'in' expression* 'end'
  declaration → type_dec | var_dec | function_dec
```

```cpp
ExpPtr Parser::parse_let() {
    Position pos = current_.pos;
    expect(TokenType::LET, "expected 'let'");

    // Parse declarations
    std::vector<DecPtr> decs;
    while (!check(TokenType::IN) && !check(TokenType::END_OF_FILE)) {
        decs.push_back(parse_declaration());
    }

    expect(TokenType::IN, "expected 'in'");

    // Parse body expressions (separated by semicolons)
    std::vector<ExpPtr> body;
    if (!check(TokenType::END)) {
        body.push_back(parse_expression());
        while (match(TokenType::SEMI)) {
            if (check(TokenType::END)) break;
            body.push_back(parse_expression());
        }
    }

    expect(TokenType::END, "expected 'end'");

    return std::make_unique<LetExp>(std::move(decs), std::move(body), pos);
}

DecPtr Parser::parse_declaration() {
    if (check(TokenType::TYPE)) {
        return parse_type_dec();
    }
    if (check(TokenType::VAR)) {
        return parse_var_dec();
    }
    if (check(TokenType::FUNCTION)) {
        return parse_function_dec();
    }

    error("expected declaration");
    advance();  // Skip bad token
    return nullptr;
}

DecPtr Parser::parse_var_dec() {
    Position pos = current_.pos;
    expect(TokenType::VAR, "expected 'var'");

    std::string name = advance().text;

    std::string type_annotation;
    if (match(TokenType::COLON)) {
        type_annotation = advance().text;
    }

    expect(TokenType::ASSIGN, "expected ':='");
    ExpPtr init = parse_expression();

    return std::make_unique<VarDec>(name, type_annotation, std::move(init), pos);
}
```

### 3.9 Parsing Identifiers (Disambiguation)

When you see an identifier, it could be:
- A simple variable: `x`
- A function call: `foo(1, 2)`
- A record creation: `Point{x = 1, y = 2}`
- An array creation: `intArray[10] of 0`
- Start of an l-value: `arr[i].field`

```cpp
ExpPtr Parser::parse_identifier_expr() {
    Position pos = current_.pos;
    std::string name = advance().text;

    // Function call: name(args)
    if (check(TokenType::LPAREN)) {
        advance();
        std::vector<ExpPtr> args;
        if (!check(TokenType::RPAREN)) {
            args.push_back(parse_expression());
            while (match(TokenType::COMMA)) {
                args.push_back(parse_expression());
            }
        }
        expect(TokenType::RPAREN, "expected ')'");
        return std::make_unique<CallExp>(name, std::move(args), pos);
    }

    // Record creation: name{field = exp, ...}
    if (check(TokenType::LBRACE)) {
        advance();
        std::vector<Field> fields;
        if (!check(TokenType::RBRACE)) {
            // Parse field = exp
            std::string field_name = advance().text;
            expect(TokenType::EQ, "expected '='");
            fields.emplace_back(field_name, parse_expression());

            while (match(TokenType::COMMA)) {
                field_name = advance().text;
                expect(TokenType::EQ, "expected '='");
                fields.emplace_back(field_name, parse_expression());
            }
        }
        expect(TokenType::RBRACE, "expected '}'");
        return std::make_unique<RecordExp>(name, std::move(fields), pos);
    }

    // Array creation: name[size] of init
    if (check(TokenType::LBRACK)) {
        advance();
        ExpPtr size = parse_expression();
        expect(TokenType::RBRACK, "expected ']'");

        if (match(TokenType::OF)) {
            ExpPtr init = parse_expression();
            return std::make_unique<ArrayExp>(name, std::move(size), std::move(init), pos);
        }

        // Not array creation, it's a subscript - build l-value
        VarPtr var = std::make_unique<SimpleVar>(name, pos);
        var = std::make_unique<SubscriptVar>(std::move(var), std::move(size), pos);
        var = parse_lvalue_suffix(std::move(var));
        return std::make_unique<VarExp>(std::move(var), pos);
    }

    // Field access or simple variable
    VarPtr var = std::make_unique<SimpleVar>(name, pos);
    if (check(TokenType::DOT) || check(TokenType::LBRACK)) {
        var = parse_lvalue_suffix(std::move(var));
    }
    return std::make_unique<VarExp>(std::move(var), pos);
}
```

### 3.10 Parsing L-value Suffixes

L-values can have chains of field accesses and subscripts:

```
arr[i].point.x[0]
```

```cpp
VarPtr Parser::parse_lvalue_suffix(VarPtr base) {
    while (true) {
        Position pos = current_.pos;

        // Field access: .name
        if (match(TokenType::DOT)) {
            std::string field = advance().text;
            base = std::make_unique<FieldVar>(std::move(base), field, pos);
            continue;
        }

        // Subscript: [exp]
        if (match(TokenType::LBRACK)) {
            ExpPtr index = parse_expression();
            expect(TokenType::RBRACK, "expected ']'");
            base = std::make_unique<SubscriptVar>(std::move(base), std::move(index), pos);
            continue;
        }

        break;
    }
    return base;
}
```

### 3.11 Error Handling

Good error messages are crucial for usability.

```cpp
void Parser::error(const std::string& msg) {
    std::ostringstream oss;
    oss << current_.pos.line << ":" << current_.pos.column
        << ": error: " << msg
        << " (got " << token_type_to_string(current_.type) << ")";
    errors_.push_back(oss.str());
}

// Synchronize: skip tokens until we find a recovery point
void Parser::synchronize() {
    while (!check(TokenType::END_OF_FILE)) {
        // Stop at statement boundaries
        switch (current_.type) {
            case TokenType::LET:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::FOR:
            case TokenType::FUNCTION:
            case TokenType::TYPE:
            case TokenType::VAR:
                return;
            default:
                advance();
        }
    }
}
```

---

## Part 4: Abstract Syntax Trees (AST)

### 4.1 What is an AST?

An **Abstract Syntax Tree** represents the hierarchical structure of the source code, abstracting away syntactic details (parentheses, semicolons, etc.).

### 4.2 Designing AST Nodes

Use a base class with a `kind` enum for type dispatch:

```cpp
enum class ExpKind {
    VAR, NIL, INT, STRING, CALL, OP, RECORD, SEQ, ASSIGN, IF, WHILE, FOR, BREAK, LET, ARRAY
};

struct Exp {
    ExpKind kind;
    Position pos;

    Exp(ExpKind k, Position p) : kind(k), pos(p) {}
    virtual ~Exp() = default;
};

// Derived types for each kind
struct IntExp : Exp {
    int value;
    IntExp(int v, Position p) : Exp(ExpKind::INT, p), value(v) {}
};

struct OpExp : Exp {
    std::unique_ptr<Exp> left;
    Op op;
    std::unique_ptr<Exp> right;

    OpExp(std::unique_ptr<Exp> l, Op o, std::unique_ptr<Exp> r, Position p)
        : Exp(ExpKind::OP, p), left(std::move(l)), op(o), right(std::move(r)) {}
};
```

### 4.3 Using Smart Pointers

Always use `std::unique_ptr` for AST nodes to ensure proper memory management:

```cpp
using ExpPtr = std::unique_ptr<Exp>;
using VarPtr = std::unique_ptr<Var>;
using DecPtr = std::unique_ptr<Dec>;
using TyPtr = std::unique_ptr<Ty>;
```

### 4.4 AST Printer (for debugging)

```cpp
class AstPrinter {
public:
    void print(const Exp& exp, int indent = 0) {
        std::string prefix(indent, ' ');

        switch (exp.kind) {
            case ExpKind::INT: {
                auto& e = static_cast<const IntExp&>(exp);
                std::cout << prefix << "IntExp: " << e.value << "\n";
                break;
            }
            case ExpKind::OP: {
                auto& e = static_cast<const OpExp&>(exp);
                std::cout << prefix << "OpExp: " << op_to_string(e.op) << "\n";
                print(*e.left, indent + 2);
                print(*e.right, indent + 2);
                break;
            }
            // ... handle other kinds
        }
    }
};
```

---

## Part 5: Putting It All Together

### 5.1 Main Program Structure

```cpp
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file>\n";
        return 1;
    }

    // Read source file
    std::string source = read_file(argv[1]);

    // Lexical analysis
    Lexer lexer(source);

    // Syntax analysis
    Parser parser(lexer);
    auto program = parser.parse();

    // Report errors
    if (lexer.has_errors() || parser.has_errors()) {
        for (const auto& err : lexer.errors()) {
            std::cerr << err << "\n";
        }
        for (const auto& err : parser.errors()) {
            std::cerr << err << "\n";
        }
        return 1;
    }

    // Success - do something with the AST
    AstPrinter printer;
    printer.print(*program);

    return 0;
}
```

### 5.2 Complete Workflow

```
┌──────────────┐    ┌───────────┐    ┌────────────┐    ┌──────────┐
│ Source Code  │───▶│   Lexer   │───▶│   Parser   │───▶│   AST    │
│  (string)    │    │ (tokens)  │    │ (grammar)  │    │ (tree)   │
└──────────────┘    └───────────┘    └────────────┘    └──────────┘
                                                             │
                                            ┌────────────────┼────────────────┐
                                            ▼                ▼                ▼
                                       ┌─────────┐    ┌───────────┐    ┌───────────┐
                                       │ Print   │    │ Type      │    │ Code Gen  │
                                       │ (debug) │    │ Check     │    │ (compile) │
                                       └─────────┘    └───────────┘    └───────────┘
```

### 5.3 Testing Your Implementation

1. **Lexer tests**: Verify tokens for various inputs
2. **Parser tests**: Verify AST structure for valid programs
3. **Error tests**: Verify meaningful errors for invalid programs

```bash
# Test lexer
echo '1 + 2 * 3' | ./mycompiler --lex

# Test parser
echo 'let var x := 10 in x + 1 end' | ./mycompiler --ast

# Test error handling
echo 'let var x 10' | ./mycompiler --parse
```

---

## Summary

### Key Concepts

1. **Lexer**: Converts characters → tokens
   - Track position for error messages
   - Handle keywords vs identifiers
   - Support escape sequences in strings
   - Handle comments (possibly nested)

2. **Parser**: Converts tokens → AST
   - One function per grammar non-terminal
   - Use precedence climbing for operators
   - Lookahead (`peek`) to decide which rule applies
   - Good error messages with position info

3. **AST**: Tree representation of program structure
   - Base class + derived classes per node type
   - Use `unique_ptr` for memory management
   - Include position info for error reporting

4. **Build System**: Automates compilation
   - Makefile manages dependencies
   - Incremental builds save time
   - Clean separation of source and build directories

### Common Pitfalls

1. **Lexer**: Forgetting to handle EOF, not tracking position correctly
2. **Parser**: Wrong precedence/associativity, not consuming tokens
3. **AST**: Memory leaks (use smart pointers!), missing position info
4. **Build**: Missing dependencies, not rebuilding when headers change
