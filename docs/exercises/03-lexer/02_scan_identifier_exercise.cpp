/**
 * Exercise 2: Scan Identifier
 *
 * Implement identifier scanning with keyword detection.
 *
 * Compile: g++ -std=c++17 -o test 02_scan_identifier_exercise.cpp
 * Run: ./test
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <cctype>

enum class TokenType {
    ID, IF, ELSE, WHILE, LET, FUNCTION, END_OF_FILE
};

struct Position {
    int line, column;
    Position(int l = 1, int c = 1) : line(l), column(c) {}
};

struct Token {
    TokenType type;
    std::string text;
    Position pos;
    Token(TokenType t, const std::string& txt, Position p)
        : type(t), text(txt), pos(p) {}
};

// Keywords map
static const std::unordered_map<std::string, TokenType> keywords = {
    {"if",       TokenType::IF},
    {"else",     TokenType::ELSE},
    {"while",    TokenType::WHILE},
    {"let",      TokenType::LET},
    {"function", TokenType::FUNCTION},
};

class Lexer {
public:
    Lexer(const std::string& source)
        : source_(source), pos_(0), line_(1), column_(1) {}

    // TODO 1: Implement peek() - return current char without advancing
    char peek() const {
        ___
    }

    // TODO 2: Implement advance() - return current char and move to next
    // Update line_ and column_ appropriately
    char advance() {
        ___
    }

    bool at_end() const {
        return pos_ >= source_.size();
    }

    // TODO 3: Implement scan_identifier()
    // - Collect alphanumeric characters and underscores
    // - Check if the result is a keyword
    // - Return appropriate token (keyword or ID)
    Token scan_identifier() {
        Position start(line_, column_);
        std::string text;

        // Collect identifier characters
        while (!at_end() && ___) {
            ___
        }

        // Check if it's a keyword
        ___

        // Return token
        ___
    }

private:
    std::string source_;
    size_t pos_;
    int line_;
    int column_;
};

const char* type_to_string(TokenType t) {
    switch (t) {
        case TokenType::ID: return "ID";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::LET: return "LET";
        case TokenType::FUNCTION: return "FUNCTION";
        default: return "?";
    }
}

int main() {
    struct TestCase {
        std::string input;
        TokenType expected_type;
        std::string expected_text;
    };

    TestCase tests[] = {
        {"hello", TokenType::ID, "hello"},
        {"if", TokenType::IF, "if"},
        {"else", TokenType::ELSE, "else"},
        {"while", TokenType::WHILE, "while"},
        {"let", TokenType::LET, "let"},
        {"function", TokenType::FUNCTION, "function"},
        {"ifelse", TokenType::ID, "ifelse"},  // Not a keyword
        {"my_var", TokenType::ID, "my_var"},
        {"x123", TokenType::ID, "x123"},
    };

    int passed = 0;
    for (const auto& test : tests) {
        Lexer lexer(test.input);
        Token tok = lexer.scan_identifier();

        bool ok = (tok.type == test.expected_type && tok.text == test.expected_text);
        if (ok) {
            passed++;
            std::cout << "✓ ";
        } else {
            std::cout << "✗ ";
        }
        std::cout << "'" << test.input << "' -> "
                  << type_to_string(tok.type) << " '" << tok.text << "'" << std::endl;
    }

    std::cout << "\nPassed: " << passed << "/" << (sizeof(tests)/sizeof(tests[0])) << std::endl;
    return 0;
}
