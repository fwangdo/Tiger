#include "Parser.hpp"
#include <sstream>

namespace tiger {

Parser::Parser(Lexer& lexer) : lexer_(lexer) {
    current_ = lexer_.next_token();
}

// ============================================================================
// Token handling
// ============================================================================

Token Parser::peek() {
    return current_;
}

Token Parser::advance() {
    Token prev = current_;
    current_ = lexer_.next_token();
    return prev;
}

bool Parser::check(TokenType type) {
    return current_.type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

void Parser::expect(TokenType type, const char* msg) {
    if (!match(type)) {
        error(msg);
    }
}

void Parser::error(const std::string& msg) {
    std::ostringstream oss;
    oss << current_.pos.line << ":" << current_.pos.column
        << ": error: " << msg;
    oss << " (got " << token_type_to_string(current_.type) << ")";
    errors_.push_back(oss.str());
}

void Parser::synchronize() {
    // Skip tokens until we find a statement boundary
    while (!check(TokenType::END_OF_FILE)) {
        switch (current_.type) {
            case TokenType::LET:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::FOR:
            case TokenType::VAR:
            case TokenType::TYPE:
            case TokenType::FUNCTION:
            case TokenType::END:
            case TokenType::IN:
                return;
            default:
                advance();
        }
    }
}

// ============================================================================
// Main entry point
// ============================================================================

std::unique_ptr<Program> Parser::parse() {
    Position pos = current_.pos;
    ExpPtr exp = parse_exp();

    if (!check(TokenType::END_OF_FILE)) {
        error("expected end of file");
    }

    return std::make_unique<Program>(std::move(exp), pos);
}

// ============================================================================
// Expression parsing with operator precedence
// Precedence (low to high):
//   1. := (assignment)
//   2. | (logical or - Tiger uses & and | for boolean ops in some variants)
//   3. & (logical and)
//   4. = <> < <= > >= (comparison)
//   5. + - (additive)
//   6. * / (multiplicative)
// ============================================================================

ExpPtr Parser::parse_exp() {
    return parse_assign_exp();
}

ExpPtr Parser::parse_assign_exp() {
    // We need to parse the left side, which might be an lvalue for assignment
    // or just an expression
    ExpPtr exp = parse_or_exp();

    if (match(TokenType::ASSIGN)) {
        Position pos = current_.pos;
        // The left side must be an lvalue
        if (exp->kind != ExpKind::VAR) {
            error("left side of assignment must be a variable");
            return exp;
        }
        VarExp* var_exp = static_cast<VarExp*>(exp.get());
        VarPtr var = std::move(var_exp->var);
        ExpPtr value = parse_exp();
        return std::make_unique<AssignExp>(std::move(var), std::move(value), pos);
    }

    return exp;
}

ExpPtr Parser::parse_or_exp() {
    // Tiger doesn't have || but uses | for OR
    // For simplicity, we'll just go to and_exp
    // (The standard Tiger doesn't have boolean operators as separate tokens)
    return parse_and_exp();
}

ExpPtr Parser::parse_and_exp() {
    // Similar to or, Tiger uses & for AND
    // For simplicity, we go straight to comparison
    return parse_comparison_exp();
}

ExpPtr Parser::parse_comparison_exp() {
    ExpPtr left = parse_add_exp();

    while (true) {
        Position pos = current_.pos;
        Op op;

        if (match(TokenType::EQ)) {
            op = Op::EQ;
        } else if (match(TokenType::NEQ)) {
            op = Op::NEQ;
        } else if (match(TokenType::LT)) {
            op = Op::LT;
        } else if (match(TokenType::LE)) {
            op = Op::LE;
        } else if (match(TokenType::GT)) {
            op = Op::GT;
        } else if (match(TokenType::GE)) {
            op = Op::GE;
        } else {
            break;
        }

        ExpPtr right = parse_add_exp();
        left = std::make_unique<OpExp>(std::move(left), op, std::move(right), pos);
    }

    return left;
}

ExpPtr Parser::parse_add_exp() {
    ExpPtr left = parse_mul_exp();

    while (true) {
        Position pos = current_.pos;
        Op op;

        if (match(TokenType::PLUS)) {
            op = Op::PLUS;
        } else if (match(TokenType::MINUS)) {
            op = Op::MINUS;
        } else {
            break;
        }

        ExpPtr right = parse_mul_exp();
        left = std::make_unique<OpExp>(std::move(left), op, std::move(right), pos);
    }

    return left;
}

ExpPtr Parser::parse_mul_exp() {
    ExpPtr left = parse_unary_exp();

    while (true) {
        Position pos = current_.pos;
        Op op;

        if (match(TokenType::STAR)) {
            op = Op::TIMES;
        } else if (match(TokenType::SLASH)) {
            op = Op::DIVIDE;
        } else {
            break;
        }

        ExpPtr right = parse_unary_exp();
        left = std::make_unique<OpExp>(std::move(left), op, std::move(right), pos);
    }

    return left;
}

ExpPtr Parser::parse_unary_exp() {
    // Handle unary minus: -exp
    if (check(TokenType::MINUS)) {
        Position pos = current_.pos;
        advance();
        ExpPtr operand = parse_unary_exp();
        // Represent as 0 - operand
        return std::make_unique<OpExp>(
            std::make_unique<IntExp>(0, pos),
            Op::MINUS,
            std::move(operand),
            pos
        );
    }

    return parse_primary_exp();
}

ExpPtr Parser::parse_primary_exp() {
    Position pos = current_.pos;

    // nil
    if (match(TokenType::NIL)) {
        return std::make_unique<NilExp>(pos);
    }

    // int literal
    if (check(TokenType::INT_LIT)) {
        Token tok = advance();
        return std::make_unique<IntExp>(tok.int_value, pos);
    }

    // string literal
    if (check(TokenType::STRING_LIT)) {
        Token tok = advance();
        return std::make_unique<StringExp>(tok.text, pos);
    }

    // if expression
    if (check(TokenType::IF)) {
        return parse_if_exp();
    }

    // while expression
    if (check(TokenType::WHILE)) {
        return parse_while_exp();
    }

    // for expression
    if (check(TokenType::FOR)) {
        return parse_for_exp();
    }

    // break
    if (match(TokenType::BREAK)) {
        return std::make_unique<BreakExp>(pos);
    }

    // let expression
    if (check(TokenType::LET)) {
        return parse_let_exp();
    }

    // parenthesized expression or sequence
    if (check(TokenType::LPAREN)) {
        return parse_seq_exp();
    }

    // identifier starting expression
    // (could be: variable, function call, record creation, array creation)
    if (check(TokenType::ID)) {
        return parse_id_exp();
    }

    error("expected expression");
    return std::make_unique<NilExp>(pos);  // error recovery
}

// ============================================================================
// Specific expression forms
// ============================================================================

ExpPtr Parser::parse_if_exp() {
    Position pos = current_.pos;
    expect(TokenType::IF, "expected 'if'");

    ExpPtr test = parse_exp();
    expect(TokenType::THEN, "expected 'then'");
    ExpPtr then_exp = parse_exp();

    ExpPtr else_exp = nullptr;
    if (match(TokenType::ELSE)) {
        else_exp = parse_exp();
    }

    return std::make_unique<IfExp>(
        std::move(test), std::move(then_exp), std::move(else_exp), pos);
}

ExpPtr Parser::parse_while_exp() {
    Position pos = current_.pos;
    expect(TokenType::WHILE, "expected 'while'");

    ExpPtr test = parse_exp();
    expect(TokenType::DO, "expected 'do'");
    ExpPtr body = parse_exp();

    return std::make_unique<WhileExp>(std::move(test), std::move(body), pos);
}

ExpPtr Parser::parse_for_exp() {
    Position pos = current_.pos;
    expect(TokenType::FOR, "expected 'for'");

    if (!check(TokenType::ID)) {
        error("expected identifier");
        return std::make_unique<NilExp>(pos);
    }
    std::string var = advance().text;

    expect(TokenType::ASSIGN, "expected ':='");
    ExpPtr lo = parse_exp();
    expect(TokenType::TO, "expected 'to'");
    ExpPtr hi = parse_exp();
    expect(TokenType::DO, "expected 'do'");
    ExpPtr body = parse_exp();

    return std::make_unique<ForExp>(
        var, std::move(lo), std::move(hi), std::move(body), pos);
}

ExpPtr Parser::parse_let_exp() {
    Position pos = current_.pos;
    expect(TokenType::LET, "expected 'let'");

    std::vector<DecPtr> decs;
    while (!check(TokenType::IN) && !check(TokenType::END_OF_FILE)) {
        DecPtr dec = parse_dec();
        if (dec) {
            decs.push_back(std::move(dec));
        }
    }

    expect(TokenType::IN, "expected 'in'");

    std::vector<ExpPtr> body;
    if (!check(TokenType::END)) {
        body.push_back(parse_exp());
        while (match(TokenType::SEMI)) {
            if (check(TokenType::END)) break;
            body.push_back(parse_exp());
        }
    }

    expect(TokenType::END, "expected 'end'");

    return std::make_unique<LetExp>(std::move(decs), std::move(body), pos);
}

ExpPtr Parser::parse_seq_exp() {
    Position pos = current_.pos;
    expect(TokenType::LPAREN, "expected '('");

    std::vector<ExpPtr> exps;
    if (!check(TokenType::RPAREN)) {
        exps.push_back(parse_exp());
        while (match(TokenType::SEMI)) {
            exps.push_back(parse_exp());
        }
    }

    expect(TokenType::RPAREN, "expected ')'");

    // Single expression in parens is just that expression
    if (exps.size() == 1) {
        return std::move(exps[0]);
    }

    return std::make_unique<SeqExp>(std::move(exps), pos);
}

// ============================================================================
// Identifier-starting expressions
// Handles: id, id(...), id{...}, id[...] of ...
// Also handles lvalue suffixes: .field, [index]
// ============================================================================

ExpPtr Parser::parse_id_exp() {
    Position pos = current_.pos;
    std::string id = advance().text;  // consume the ID

    // Function call: id ( args )
    if (check(TokenType::LPAREN)) {
        advance();  // consume '('
        std::vector<ExpPtr> args;

        if (!check(TokenType::RPAREN)) {
            args.push_back(parse_exp());
            while (match(TokenType::COMMA)) {
                args.push_back(parse_exp());
            }
        }

        expect(TokenType::RPAREN, "expected ')'");
        return std::make_unique<CallExp>(id, std::move(args), pos);
    }

    // Record creation: id { field = exp, ... }
    if (check(TokenType::LBRACE)) {
        advance();  // consume '{'
        std::vector<Field> fields;

        if (!check(TokenType::RBRACE)) {
            // field = exp
            Position field_pos = current_.pos;
            if (!check(TokenType::ID)) {
                error("expected field name");
            }
            std::string field_name = advance().text;
            expect(TokenType::EQ, "expected '='");
            ExpPtr field_exp = parse_exp();
            fields.emplace_back(field_name, std::move(field_exp), field_pos);

            while (match(TokenType::COMMA)) {
                field_pos = current_.pos;
                if (!check(TokenType::ID)) {
                    error("expected field name");
                }
                field_name = advance().text;
                expect(TokenType::EQ, "expected '='");
                field_exp = parse_exp();
                fields.emplace_back(field_name, std::move(field_exp), field_pos);
            }
        }

        expect(TokenType::RBRACE, "expected '}'");
        return std::make_unique<RecordExp>(id, std::move(fields), pos);
    }

    // Array creation or subscript: id [ exp ] ...
    if (check(TokenType::LBRACK)) {
        advance();  // consume '['
        ExpPtr index_exp = parse_exp();
        expect(TokenType::RBRACK, "expected ']'");

        // Array creation: id [ size ] of init
        if (match(TokenType::OF)) {
            ExpPtr init = parse_exp();
            return std::make_unique<ArrayExp>(
                id, std::move(index_exp), std::move(init), pos);
        }

        // Otherwise it's a subscript - build an lvalue
        VarPtr var = std::make_unique<SimpleVar>(id, pos);
        var = std::make_unique<SubscriptVar>(
            std::move(var), std::move(index_exp), pos);

        // Continue parsing lvalue suffixes
        var = parse_lvalue_suffix(std::move(var));

        return std::make_unique<VarExp>(std::move(var), pos);
    }

    // Field access: id . field ...
    if (check(TokenType::DOT)) {
        VarPtr var = std::make_unique<SimpleVar>(id, pos);
        var = parse_lvalue_suffix(std::move(var));
        return std::make_unique<VarExp>(std::move(var), pos);
    }

    // Just a simple variable
    VarPtr var = std::make_unique<SimpleVar>(id, pos);
    return std::make_unique<VarExp>(std::move(var), pos);
}

VarPtr Parser::parse_lvalue_suffix(VarPtr base) {
    while (true) {
        Position pos = current_.pos;

        // Field access: .id
        if (match(TokenType::DOT)) {
            if (!check(TokenType::ID)) {
                error("expected field name");
                return base;
            }
            std::string field = advance().text;
            base = std::make_unique<FieldVar>(std::move(base), field, pos);
            continue;
        }

        // Subscript: [exp]
        if (match(TokenType::LBRACK)) {
            ExpPtr index = parse_exp();
            expect(TokenType::RBRACK, "expected ']'");
            base = std::make_unique<SubscriptVar>(std::move(base), std::move(index), pos);
            continue;
        }

        break;
    }

    return base;
}

// ============================================================================
// Declarations
// ============================================================================

DecPtr Parser::parse_dec() {
    if (check(TokenType::TYPE)) {
        return parse_type_dec();
    }
    if (check(TokenType::VAR)) {
        return parse_var_dec();
    }
    if (check(TokenType::FUNCTION)) {
        return parse_function_dec();
    }

    error("expected declaration");
    advance();  // skip bad token
    return nullptr;
}

DecPtr Parser::parse_type_dec() {
    Position pos = current_.pos;
    expect(TokenType::TYPE, "expected 'type'");

    if (!check(TokenType::ID)) {
        error("expected type name");
        return nullptr;
    }
    std::string name = advance().text;

    expect(TokenType::EQ, "expected '='");

    TyPtr ty = parse_ty();

    return std::make_unique<TypeDec>(name, std::move(ty), pos);
}

DecPtr Parser::parse_var_dec() {
    Position pos = current_.pos;
    expect(TokenType::VAR, "expected 'var'");

    if (!check(TokenType::ID)) {
        error("expected variable name");
        return nullptr;
    }
    std::string name = advance().text;

    std::string type_id;
    if (match(TokenType::COLON)) {
        if (!check(TokenType::ID)) {
            error("expected type name");
        } else {
            type_id = advance().text;
        }
    }

    expect(TokenType::ASSIGN, "expected ':='");
    ExpPtr init = parse_exp();

    return std::make_unique<VarDec>(name, type_id, std::move(init), pos);
}

DecPtr Parser::parse_function_dec() {
    Position pos = current_.pos;
    expect(TokenType::FUNCTION, "expected 'function'");

    if (!check(TokenType::ID)) {
        error("expected function name");
        return nullptr;
    }
    std::string name = advance().text;

    expect(TokenType::LPAREN, "expected '('");
    std::vector<TypeField> params = parse_type_fields();
    expect(TokenType::RPAREN, "expected ')'");

    std::string result_type;
    if (match(TokenType::COLON)) {
        if (!check(TokenType::ID)) {
            error("expected return type");
        } else {
            result_type = advance().text;
        }
    }

    expect(TokenType::EQ, "expected '='");
    ExpPtr body = parse_exp();

    return std::make_unique<FunctionDec>(
        name, std::move(params), result_type, std::move(body), pos);
}

// ============================================================================
// Types
// ============================================================================

TyPtr Parser::parse_ty() {
    Position pos = current_.pos;

    // Record type: { fields }
    if (match(TokenType::LBRACE)) {
        std::vector<TypeField> fields = parse_type_fields();
        expect(TokenType::RBRACE, "expected '}'");
        return std::make_unique<RecordTy>(std::move(fields), pos);
    }

    // Array type: array of id
    if (match(TokenType::ARRAY)) {
        expect(TokenType::OF, "expected 'of'");
        if (!check(TokenType::ID)) {
            error("expected type name");
            return std::make_unique<NameTy>("error", pos);
        }
        std::string element_type = advance().text;
        return std::make_unique<ArrayTy>(element_type, pos);
    }

    // Name type: id
    if (check(TokenType::ID)) {
        std::string name = advance().text;
        return std::make_unique<NameTy>(name, pos);
    }

    error("expected type");
    return std::make_unique<NameTy>("error", pos);
}

std::vector<TypeField> Parser::parse_type_fields() {
    std::vector<TypeField> fields;

    if (!check(TokenType::ID)) {
        return fields;  // empty
    }

    Position pos = current_.pos;
    std::string name = advance().text;
    expect(TokenType::COLON, "expected ':'");
    if (!check(TokenType::ID)) {
        error("expected type name");
        return fields;
    }
    std::string type_id = advance().text;
    fields.emplace_back(name, type_id, pos);

    while (match(TokenType::COMMA)) {
        pos = current_.pos;
        if (!check(TokenType::ID)) {
            error("expected field name");
            break;
        }
        name = advance().text;
        expect(TokenType::COLON, "expected ':'");
        if (!check(TokenType::ID)) {
            error("expected type name");
            break;
        }
        type_id = advance().text;
        fields.emplace_back(name, type_id, pos);
    }

    return fields;
}

} // namespace tiger
