#include "lexer.hpp"
#include <unordered_map>
#include <sstream>

namespace tiger {

static const std::unordered_map<std::string, TokenType> keywords = {
    {"array",    TokenType::ARRAY},
    {"break",    TokenType::BREAK},
    {"do",       TokenType::DO},
    {"else",     TokenType::ELSE},
    {"end",      TokenType::END},
    {"for",      TokenType::FOR},
    {"function", TokenType::FUNCTION},
    {"if",       TokenType::IF},
    {"in",       TokenType::IN},
    {"let",      TokenType::LET},
    {"nil",      TokenType::NIL},
    {"of",       TokenType::OF},
    {"then",     TokenType::THEN},
    {"to",       TokenType::TO},
    {"type",     TokenType::TYPE},
    {"var",      TokenType::VAR},
    {"while",    TokenType::WHILE},
};

Lexer::Lexer(const std::string& source)
    : source_(source), pos_(0), line_(1), column_(1), has_current_(false) {}

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

void Lexer::add_error(const std::string& msg) {
    std::ostringstream oss;
    oss << line_ << ":" << column_ << ": " << msg;
    errors_.push_back(oss.str());
}

Token Lexer::make_token(TokenType type, const std::string& text, Position start) {
    return Token(type, text, start);
}

// Tiger comments are /* ... */ and can nest
// This is the right answer for handling comments. 
bool Lexer::skip_comment() {
    if (peek() != '/' || peek_next() != '*') return false;

    Position start(line_, column_);
    advance(); // /
    advance(); // *

    int depth = 1;
    while (!at_end() && depth > 0) {
        if (peek() == '/' && peek_next() == '*') {
            advance();
            advance();
            depth++;
        } else if (peek() == '*' && peek_next() == '/') {
            advance();
            advance();
            depth--;
        } else {
            advance();
        }
    }

    if (depth > 0) {
        add_error("unterminated comment");
    }
    return true;
}

void Lexer::skip_whitespace_and_comments() {
    while (!at_end()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            advance();
        } else if (c == '/' && peek_next() == '*') {
            skip_comment();
        } else {
            break;
        }
    }
}

Token Lexer::scan_identifier() {
    Position start(line_, column_);
    std::string text;

    while (!at_end()) {
        char c = peek();
        if (std::isalnum(c) || c == '_') {
            text += advance();
        } else {
            break;
        }
    }

    // check whether it's reserved or not. 
    auto it = keywords.find(text);
    if (it != keywords.end()) {
        return make_token(it->second, text, start);
    }
    return make_token(TokenType::ID, text, start);
}

Token Lexer::scan_number() {
    Position start(line_, column_);
    std::string text;

    while (!at_end() && std::isdigit(peek())) {
        text += advance();
    }

    Token tok = make_token(TokenType::INT_LIT, text, start);
    tok.int_value = std::stoi(text);
    return tok;
}

Token Lexer::scan_string() {
    Position start(line_, column_);
    advance(); // opening "

    std::string text;
    std::string raw = "\"";

    while (!at_end() && peek() != '"') {
        char c = peek();
        if (c == '\\') {
            raw += advance();
            if (at_end()) {
                add_error("unterminated string");
                return make_token(TokenType::ERROR, raw, start);
            }
            char escaped = advance();
            raw += escaped;
            switch (escaped) {
                case 'n':  text += '\n'; break;
                case 't':  text += '\t'; break;
                case 'r':  text += '\r'; break;
                case '\\': text += '\\'; break;
                case '"':  text += '"'; break;
                default:
                    add_error(std::string("unknown escape sequence: \\") + escaped);
                    text += escaped;
            }
        } else if (c == '\n') {
            add_error("newline in string literal");
            break;
        } else {
            text += advance();
            raw += c;
        }
    }

    if (at_end() || peek() != '"') {
        add_error("unterminated string");
        return make_token(TokenType::ERROR, raw, start);
    }

    advance(); // closing "
    raw += '"';

    Token tok = make_token(TokenType::STRING_LIT, text, start);
    return tok;
}

Token Lexer::next_token() {
    // if there is lookahead token, then it returns lookahead.
    if (has_current_) {
        has_current_ = false;
        return current_;
    }

    skip_whitespace_and_comments();

    if (at_end()) {
        return make_token(TokenType::END_OF_FILE, "", Position(line_, column_));
    }

    Position start(line_, column_);
    char c = peek();

    // Identifiers and keywords
    if (std::isalpha(c)) {
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

    // Operators and punctuation
    advance();

    switch (c) {
        case '+': return make_token(TokenType::PLUS, "+", start);
        case '-': return make_token(TokenType::MINUS, "-", start);
        case '*': return make_token(TokenType::STAR, "*", start);
        case '/': return make_token(TokenType::SLASH, "/", start);
        case '.': return make_token(TokenType::DOT, ".", start);
        case ',': return make_token(TokenType::COMMA, ",", start);
        case ';': return make_token(TokenType::SEMI, ";", start);
        case '(': return make_token(TokenType::LPAREN, "(", start);
        case ')': return make_token(TokenType::RPAREN, ")", start);
        case '[': return make_token(TokenType::LBRACK, "[", start);
        case ']': return make_token(TokenType::RBRACK, "]", start);
        case '{': return make_token(TokenType::LBRACE, "{", start);
        case '}': return make_token(TokenType::RBRACE, "}", start);

        case '=': return make_token(TokenType::EQ, "=", start);

        case '<':
            if (peek() == '>') {
                advance();
                return make_token(TokenType::NEQ, "<>", start);
            }
            if (peek() == '=') {
                advance();
                return make_token(TokenType::LE, "<=", start);
            }
            // otherwise.. 
            return make_token(TokenType::LT, "<", start);

        case '>':
            if (peek() == '=') {
                advance();
                return make_token(TokenType::GE, ">=", start);
            }
            return make_token(TokenType::GT, ">", start);

        case ':':
            if (peek() == '=') {
                advance();
                return make_token(TokenType::ASSIGN, ":=", start);
            }
            return make_token(TokenType::COLON, ":", start);

        default:
            add_error(std::string("unexpected character: ") + c);
            return make_token(TokenType::ERROR, std::string(1, c), start);
    }
}

Token Lexer::peek_token() {
    if (!has_current_) {
        current_ = next_token();
        has_current_ = true;
    }
    return current_;
}

} // namespace tiger
