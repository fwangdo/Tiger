/**
 * Exercise 1: Token Handling
 *
 * Implement the basic token handling methods for a parser.
 *
 * Compile: g++ -std=c++17 -o test 01_token_handling_exercise.cpp
 * Run: ./test
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

enum class TokenType {
    INT_LIT, ID, PLUS, MINUS, STAR, LPAREN, RPAREN, SEMICOLON, END_OF_FILE
};

struct Token {
    TokenType type;
    std::string text;
    Token(TokenType t, const std::string& txt = "") : type(t), text(txt) {}
};

const char* type_str(TokenType t) {
    switch (t) {
        case TokenType::INT_LIT: return "INT";
        case TokenType::ID: return "ID";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::STAR: return "STAR";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::SEMICOLON: return "SEMI";
        case TokenType::END_OF_FILE: return "EOF";
        default: return "?";
    }
}

// Simple mock lexer that returns tokens from a vector
class MockLexer {
public:
    MockLexer(std::vector<Token> tokens) : tokens_(std::move(tokens)), pos_(0) {}

    Token next_token() {
        if (pos_ >= tokens_.size()) {
            return Token(TokenType::END_OF_FILE);
        }
        return tokens_[pos_++];
    }

private:
    std::vector<Token> tokens_;
    size_t pos_;
};

class Parser {
public:
    Parser(MockLexer& lexer) : lexer_(lexer) {
        // TODO 1: Initialize current_ with the first token from lexer
        ___
    }

    // TODO 2: Implement peek() - return current token without consuming
    Token peek() {
        ___
    }

    // TODO 3: Implement advance() - return current token and get next
    Token advance() {
        ___
    }

    // TODO 4: Implement check() - return true if current token matches type
    bool check(TokenType type) {
        ___
    }

    // TODO 5: Implement match() - if current matches, advance and return true
    bool match(TokenType type) {
        ___
    }

    // TODO 6: Implement expect() - if current matches, advance; otherwise throw
    void expect(TokenType type, const char* msg) {
        ___
    }

    // Test method to consume all tokens
    void consume_all() {
        while (!check(TokenType::END_OF_FILE)) {
            Token t = advance();
            std::cout << "Consumed: " << type_str(t.type);
            if (!t.text.empty()) std::cout << " '" << t.text << "'";
            std::cout << std::endl;
        }
    }

private:
    MockLexer& lexer_;
    Token current_{TokenType::END_OF_FILE};
};

int main() {
    // Test tokens: 1 + 2 * 3 ;
    std::vector<Token> tokens = {
        Token(TokenType::INT_LIT, "1"),
        Token(TokenType::PLUS, "+"),
        Token(TokenType::INT_LIT, "2"),
        Token(TokenType::STAR, "*"),
        Token(TokenType::INT_LIT, "3"),
        Token(TokenType::SEMICOLON, ";"),
        Token(TokenType::END_OF_FILE),
    };

    MockLexer lexer(tokens);
    Parser parser(lexer);

    std::cout << "=== Testing peek ===" << std::endl;
    std::cout << "Current: " << type_str(parser.peek().type) << std::endl;

    std::cout << "\n=== Testing check ===" << std::endl;
    std::cout << "Is INT_LIT? " << (parser.check(TokenType::INT_LIT) ? "yes" : "no") << std::endl;
    std::cout << "Is PLUS? " << (parser.check(TokenType::PLUS) ? "yes" : "no") << std::endl;

    std::cout << "\n=== Testing match ===" << std::endl;
    std::cout << "Match INT_LIT: " << (parser.match(TokenType::INT_LIT) ? "yes" : "no") << std::endl;
    std::cout << "Current after match: " << type_str(parser.peek().type) << std::endl;

    std::cout << "\n=== Testing expect ===" << std::endl;
    try {
        parser.expect(TokenType::PLUS, "expected '+'");
        std::cout << "✓ Expect PLUS succeeded" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "✗ " << e.what() << std::endl;
    }

    std::cout << "\n=== Consuming remaining ===" << std::endl;
    parser.consume_all();

    std::cout << "\n✓ Token handling exercise complete!" << std::endl;
    return 0;
}
