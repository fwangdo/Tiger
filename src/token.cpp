#include "token.hpp"

namespace tiger {

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TokenType::INT_LIT:    return "INT_LIT";
        case TokenType::STRING_LIT: return "STRING_LIT";
        case TokenType::ID:         return "ID";
        case TokenType::ARRAY:      return "ARRAY";
        case TokenType::BREAK:      return "BREAK";
        case TokenType::DO:         return "DO";
        case TokenType::ELSE:       return "ELSE";
        case TokenType::END:        return "END";
        case TokenType::FOR:        return "FOR";
        case TokenType::FUNCTION:   return "FUNCTION";
        case TokenType::IF:         return "IF";
        case TokenType::IN:         return "IN";
        case TokenType::LET:        return "LET";
        case TokenType::NIL:        return "NIL";
        case TokenType::OF:         return "OF";
        case TokenType::THEN:       return "THEN";
        case TokenType::TO:         return "TO";
        case TokenType::TYPE:       return "TYPE";
        case TokenType::VAR:        return "VAR";
        case TokenType::WHILE:      return "WHILE";
        case TokenType::PLUS:       return "PLUS";
        case TokenType::MINUS:      return "MINUS";
        case TokenType::STAR:       return "STAR";
        case TokenType::SLASH:      return "SLASH";
        case TokenType::EQ:         return "EQ";
        case TokenType::NEQ:        return "NEQ";
        case TokenType::LT:         return "LT";
        case TokenType::LE:         return "LE";
        case TokenType::GT:         return "GT";
        case TokenType::GE:         return "GE";
        case TokenType::ASSIGN:     return "ASSIGN";
        case TokenType::DOT:        return "DOT";
        case TokenType::COMMA:      return "COMMA";
        case TokenType::SEMI:       return "SEMI";
        case TokenType::COLON:      return "COLON";
        case TokenType::LPAREN:     return "LPAREN";
        case TokenType::RPAREN:     return "RPAREN";
        case TokenType::LBRACK:     return "LBRACK";
        case TokenType::RBRACK:     return "RBRACK";
        case TokenType::LBRACE:     return "LBRACE";
        case TokenType::RBRACE:     return "RBRACE";
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::ERROR:      return "ERROR";
    }
    return "UNKNOWN";
}

std::ostream& operator<<(std::ostream& os, const Token& tok) {
    os << token_type_to_string(tok.type);
    if (!tok.text.empty()) {
        os << "(" << tok.text << ")";
    }
    os << " at " << tok.pos.line << ":" << tok.pos.column;
    return os;
}

} // namespace tiger
