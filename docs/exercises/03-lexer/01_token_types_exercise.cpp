/**
 * Exercise 1: Token Types
 *
 * Define the token types and token structure for a simple language.
 *
 * Compile: g++ -std=c++17 -o test 01_token_types_exercise.cpp
 * Run: ./test
 */

#include <iostream>
#include <string>

// TODO 1: Define the TokenType enum
// Include at least: INT_LIT, STRING_LIT, ID, PLUS, MINUS, STAR, SLASH,
//                   LPAREN, RPAREN, ASSIGN, SEMICOLON, END_OF_FILE, ERROR
enum class TokenType {
    // Add token types here
    ___
};

// TODO 2: Define the Position struct
// Fields: line (int), column (int)
// Constructor that takes line and column with defaults of 1
struct Position {
    ___
};

// TODO 3: Define the Token struct
// Fields: type (TokenType), text (std::string), pos (Position)
// Optional: int_value for integer literals
struct Token {
    ___

    // Constructor
    Token(TokenType t, const std::string& txt, Position p)
        : ___ {}
};

// TODO 4: Implement token_type_to_string function
// Return a string representation of the token type
const char* token_type_to_string(TokenType type) {
    switch (type) {
        // Add cases here
        ___
        default: return "UNKNOWN";
    }
}

// ========== Test ==========
int main() {
    // Create some test tokens
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
