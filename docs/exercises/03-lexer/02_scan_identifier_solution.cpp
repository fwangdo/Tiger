/**
 * Exercise 2: Scan Identifier - SOLUTION
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

    // TODO 1: Implement peek()
    char peek() const {
        if (pos_ >= source_.size()) return '\0';
        return source_[pos_];
    }

    // TODO 2: Implement advance()
    char advance() {
        char c = source_[pos_++];
        if (c == '\n') {
            line_++;
            column_ = 1;
        } else {
            column_++;
        }
        return c;
    }

    bool at_end() const {
        return pos_ >= source_.size();
    }

    // TODO 3: Implement scan_identifier()
    Token scan_identifier() {
        Position start(line_, column_);
        std::string text;

        // Collect identifier characters
        while (!at_end() && (std::isalnum(peek()) || peek() == '_')) {
            text += advance();
        }

        // Check if it's a keyword
        auto it = keywords.find(text);
        if (it != keywords.end()) {
            return Token(it->second, text, start);
        }

        // Return identifier token
        return Token(TokenType::ID, text, start);
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
        {"ifelse", TokenType::ID, "ifelse"},
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
