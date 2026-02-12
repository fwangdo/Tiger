#ifndef TIGER_TOKEN_HPP
#define TIGER_TOKEN_HPP

#include <string>
#include <ostream>

namespace tiger {

enum class TokenType {
    // Literals
    INT_LIT,
    STRING_LIT,

    // Identifier
    ID,

    // Keywords
    ARRAY,
    BREAK,
    DO,
    ELSE,
    END,
    FOR,
    FUNCTION,
    IF,
    IN,
    LET,
    NIL,
    OF,
    THEN,
    TO,
    TYPE,
    VAR,
    WHILE,

    // Operators
    PLUS,       // +
    MINUS,      // -
    STAR,       // *
    SLASH,      // /
    EQ,         // =
    NEQ,        // <>
    LT,         // <
    LE,         // <=
    GT,         // >
    GE,         // >=
    ASSIGN,     // :=

    // Punctuation
    DOT,        // .
    COMMA,      // ,
    SEMI,       // ;
    COLON,      // :
    LPAREN,     // (
    RPAREN,     // )
    LBRACK,     // [
    RBRACK,     // ]
    LBRACE,     // {
    RBRACE,     // }

    // Special
    END_OF_FILE,
    ERROR,
};

const char* token_type_to_string(TokenType type);

struct Position {
    int line;
    int column;

    Position() : line(1), column(1) {}
    Position(int l, int c) : line(l), column(c) {}
};

struct Token {
    TokenType type;
    std::string text;
    Position pos;

    // For INT_LIT
    int int_value;

    Token() : type(TokenType::ERROR), int_value(0) {}
    Token(TokenType t, const std::string& txt, Position p)
        : type(t), text(txt), pos(p), int_value(0) {}
};

std::ostream& operator<<(std::ostream& os, const Token& tok);

} // namespace tiger

#endif // TIGER_TOKEN_HPP
