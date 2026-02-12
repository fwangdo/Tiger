#ifndef TIGER_LEXER_HPP
#define TIGER_LEXER_HPP

#include "Token.hpp"
#include <string>
#include <vector>

namespace tiger {

class Lexer {
public:
    explicit Lexer(const std::string& source);

    Token next_token();
    Token peek_token();
    bool at_end() const;

    // the first const means "this function returns constant."
    // the second const means "this fucntino won't change this instance." 
    // & -> no copy. 
    const std::vector<std::string>& errors() const { return errors_; }
    bool has_errors() const { return !errors_.empty(); }

private:
    std::string source_;
    size_t pos_;
    int line_;
    int column_;
    Token current_;
    bool has_current_; // means "does it have lookahead?"
    std::vector<std::string> errors_;

    char peek() const;
    char peek_next() const;
    char advance();
    void skip_whitespace_and_comments();
    bool skip_comment();

    Token make_token(TokenType type, const std::string& text, Position start);
    Token scan_identifier();
    Token scan_number();
    Token scan_string();
    void add_error(const std::string& msg);
};

} // namespace tiger

#endif // TIGER_LEXER_HPP
