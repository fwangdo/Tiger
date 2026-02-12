#ifndef TIGER_PARSER_HPP
#define TIGER_PARSER_HPP

#include "AST.hpp"
#include "lexer/Lexer.hpp"
#include <memory>
#include <vector>

namespace tiger {

class Parser {
public:
    explicit Parser(Lexer& lexer);

    std::unique_ptr<Program> parse();

    const std::vector<std::string>& errors() const { return errors_; }
    bool has_errors() const { return !errors_.empty(); }

private:
    Lexer& lexer_;
    Token current_;
    std::vector<std::string> errors_;

    // Token handling
    Token peek();
    Token advance();
    bool check(TokenType type);
    bool match(TokenType type);
    void expect(TokenType type, const char* msg);

    // Error handling
    void error(const std::string& msg);
    void synchronize();

    // Expression parsing with precedence climbing
    ExpPtr parse_exp();
    ExpPtr parse_assign_exp();
    ExpPtr parse_or_exp();
    ExpPtr parse_and_exp();
    ExpPtr parse_comparison_exp();
    ExpPtr parse_add_exp();
    ExpPtr parse_mul_exp();
    ExpPtr parse_unary_exp();
    ExpPtr parse_primary_exp();

    // Specific expression forms
    ExpPtr parse_if_exp();
    ExpPtr parse_while_exp();
    ExpPtr parse_for_exp();
    ExpPtr parse_let_exp();
    ExpPtr parse_seq_exp();

    // L-values and identifiers starting expressions
    // (handles: id, id(...), id{...}, id[...] of ..., lvalue)
    ExpPtr parse_id_exp();
    VarPtr parse_lvalue_suffix(VarPtr base);

    // Declarations
    DecPtr parse_dec();
    DecPtr parse_type_dec();
    DecPtr parse_var_dec();
    DecPtr parse_function_dec();

    // Types
    TyPtr parse_ty();
    std::vector<TypeField> parse_type_fields();
};

} // namespace tiger

#endif // TIGER_PARSER_HPP
