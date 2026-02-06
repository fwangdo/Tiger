/**
 * Exercise 1: Token Types - SOLUTION
 */

#include <iostream>
#include <string>

// TODO 1: Define the TokenType enum
enum class TokenType {
    INT_LIT,
    STRING_LIT,
    ID,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    LPAREN,
    RPAREN,
    ASSIGN,
    SEMICOLON,
    END_OF_FILE,
    ERROR
};

// TODO 2: Define the Position struct
struct Position {
    int line;
    int column;

    Position(int l = 1, int c = 1) : line(l), column(c) {}
};

// TODO 3: Define the Token struct
struct Token {
    TokenType type;
    std::string text;
    Position pos;
    int int_value;

    Token(TokenType t, const std::string& txt, Position p)
        : type(t), text(txt), pos(p), int_value(0) {}
};

// TODO 4: Implement token_type_to_string function
const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TokenType::INT_LIT:     return "INT_LIT";
        case TokenType::STRING_LIT:  return "STRING_LIT";
        case TokenType::ID:          return "ID";
        case TokenType::PLUS:        return "PLUS";
        case TokenType::MINUS:       return "MINUS";
        case TokenType::STAR:        return "STAR";
        case TokenType::SLASH:       return "SLASH";
        case TokenType::LPAREN:      return "LPAREN";
        case TokenType::RPAREN:      return "RPAREN";
        case TokenType::ASSIGN:      return "ASSIGN";
        case TokenType::SEMICOLON:   return "SEMICOLON";
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::ERROR:       return "ERROR";
        default: return "UNKNOWN";
    }
}

// ========== Test ==========
int main() {
    Token t1(TokenType::INT_LIT, "42", Position(1, 1));
    Token t2(TokenType::PLUS, "+", Position(1, 4));
    Token t3(TokenType::ID, "foo", Position(1, 6));

    std::cout << "Token 1: " << token_type_to_string(t1.type)
              << " '" << t1.text << "' at "
              << t1.pos.line << ":" << t1.pos.column << std::endl;

    std::cout << "Token 2: " << token_type_to_string(t2.type)
              << " '" << t2.text << "'" << std::endl;

    std::cout << "Token 3: " << token_type_to_string(t3.type)
              << " '" << t3.text << "'" << std::endl;

    std::cout << "\n✓ Token types exercise complete!" << std::endl;
    return 0;
}
