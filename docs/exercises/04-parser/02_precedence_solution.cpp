/**
 * Exercise 2: Operator Precedence Parsing - SOLUTION
 */

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <sstream>

enum class TokenType {
    INT_LIT, PLUS, MINUS, STAR, SLASH, LPAREN, RPAREN, END_OF_FILE
};

struct Token {
    TokenType type;
    std::string text;
    int value;
    Token(TokenType t, const std::string& txt = "", int v = 0)
        : type(t), text(txt), value(v) {}
};

struct Expr {
    virtual ~Expr() = default;
    virtual int eval() const = 0;
    virtual std::string to_string() const = 0;
};

using ExprPtr = std::unique_ptr<Expr>;

struct IntExpr : Expr {
    int value;
    IntExpr(int v) : value(v) {}
    int eval() const override { return value; }
    std::string to_string() const override { return std::to_string(value); }
};

struct BinOpExpr : Expr {
    ExprPtr left;
    char op;
    ExprPtr right;

    BinOpExpr(ExprPtr l, char o, ExprPtr r)
        : left(std::move(l)), op(o), right(std::move(r)) {}

    int eval() const override {
        int l = left->eval(), r = right->eval();
        switch (op) {
            case '+': return l + r;
            case '-': return l - r;
            case '*': return l * r;
            case '/': return r != 0 ? l / r : 0;
            default: return 0;
        }
    }

    std::string to_string() const override {
        return "(" + left->to_string() + " " + op + " " + right->to_string() + ")";
    }
};

class Lexer {
public:
    Lexer(const std::string& src) : src_(src), pos_(0) {}

    Token next() {
        skip_whitespace();
        if (pos_ >= src_.size()) return Token(TokenType::END_OF_FILE);

        char c = src_[pos_++];
        switch (c) {
            case '+': return Token(TokenType::PLUS, "+");
            case '-': return Token(TokenType::MINUS, "-");
            case '*': return Token(TokenType::STAR, "*");
            case '/': return Token(TokenType::SLASH, "/");
            case '(': return Token(TokenType::LPAREN, "(");
            case ')': return Token(TokenType::RPAREN, ")");
            default:
                if (std::isdigit(c)) {
                    pos_--;
                    return scan_number();
                }
                return Token(TokenType::END_OF_FILE);
        }
    }

private:
    void skip_whitespace() {
        while (pos_ < src_.size() && std::isspace(src_[pos_])) pos_++;
    }

    Token scan_number() {
        std::string text;
        while (pos_ < src_.size() && std::isdigit(src_[pos_])) {
            text += src_[pos_++];
        }
        return Token(TokenType::INT_LIT, text, std::stoi(text));
    }

    std::string src_;
    size_t pos_;
};

class Parser {
public:
    Parser(Lexer& lexer) : lexer_(lexer), current_(lexer_.next()) {}

    ExprPtr parse() {
        return parse_expression();
    }

private:
    Token peek() { return current_; }
    Token advance() { Token t = current_; current_ = lexer_.next(); return t; }
    bool check(TokenType t) { return current_.type == t; }
    bool match(TokenType t) { if (check(t)) { advance(); return true; } return false; }

    ExprPtr parse_expression() {
        return parse_additive();
    }

    // TODO 1: parse_additive()
    ExprPtr parse_additive() {
        ExprPtr left = parse_multiplicative();

        while (check(TokenType::PLUS) || check(TokenType::MINUS)) {
            char op;
            if (match(TokenType::PLUS)) {
                op = '+';
            } else if (match(TokenType::MINUS)) {
                op = '-';
            } else {
                break;
            }

            ExprPtr right = parse_multiplicative();
            left = std::make_unique<BinOpExpr>(std::move(left), op, std::move(right));
        }

        return left;
    }

    // TODO 2: parse_multiplicative()
    ExprPtr parse_multiplicative() {
        ExprPtr left = parse_primary();

        while (check(TokenType::STAR) || check(TokenType::SLASH)) {
            char op;
            if (match(TokenType::STAR)) {
                op = '*';
            } else if (match(TokenType::SLASH)) {
                op = '/';
            } else {
                break;
            }

            ExprPtr right = parse_primary();
            left = std::make_unique<BinOpExpr>(std::move(left), op, std::move(right));
        }

        return left;
    }

    // TODO 3: parse_primary()
    ExprPtr parse_primary() {
        if (check(TokenType::INT_LIT)) {
            Token t = advance();
            return std::make_unique<IntExpr>(t.value);
        }

        if (match(TokenType::LPAREN)) {
            ExprPtr expr = parse_expression();
            if (!match(TokenType::RPAREN)) {
                throw std::runtime_error("expected ')'");
            }
            return expr;
        }

        throw std::runtime_error("expected expression");
    }

    Lexer& lexer_;
    Token current_;
};

int main() {
    struct TestCase {
        std::string input;
        int expected;
    };

    TestCase tests[] = {
        {"1 + 2", 3},
        {"5 - 3", 2},
        {"2 * 3", 6},
        {"6 / 2", 3},
        {"1 + 2 * 3", 7},
        {"(1 + 2) * 3", 9},
        {"10 - 2 - 3", 5},
        {"2 * 3 + 4 * 5", 26},
    };

    int passed = 0;
    for (const auto& test : tests) {
        try {
            Lexer lexer(test.input);
            Parser parser(lexer);
            ExprPtr expr = parser.parse();

            int result = expr->eval();
            bool ok = (result == test.expected);

            if (ok) {
                passed++;
                std::cout << "✓ ";
            } else {
                std::cout << "✗ ";
            }
            std::cout << test.input << " = " << result
                      << " (expected " << test.expected << ")"
                      << " [" << expr->to_string() << "]" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "✗ " << test.input << " - Error: " << e.what() << std::endl;
        }
    }

    std::cout << "\nPassed: " << passed << "/" << (sizeof(tests)/sizeof(tests[0])) << std::endl;
    return 0;
}
