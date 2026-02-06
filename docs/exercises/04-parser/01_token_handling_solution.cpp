/**
 * Exercise 1: Token Handling - SOLUTION
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
        // TODO 1: Initialize current_ with the first token
        current_ = lexer_.next_token();
    }

    // TODO 2: Implement peek()
    Token peek() {
        return current_;
    }

    // TODO 3: Implement advance()
    Token advance() {
        Token prev = current_;
        current_ = lexer_.next_token();
        return prev;
    }

    // TODO 4: Implement check()
    bool check(TokenType type) {
        return current_.type == type;
    }

    // TODO 5: Implement match()
    bool match(TokenType type) {
        if (check(type)) {
            advance();
            return true;
        }
        return false;
    }

    // TODO 6: Implement expect()
    void expect(TokenType type, const char* msg) {
        if (!match(type)) {
            throw std::runtime_error(msg);
        }
    }

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
