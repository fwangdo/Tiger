#ifndef TIGER_AST_PRINTER_HPP
#define TIGER_AST_PRINTER_HPP

#include "ast.hpp"
#include <ostream>

namespace tiger {

class AstPrinter {
public:
    explicit AstPrinter(std::ostream& os) : os_(os), indent_(0) {}

    void print(const Program& prog);
    void print(const Exp& exp);
    void print(const Var& var);
    void print(const Dec& dec);
    void print(const Ty& ty);

private:
    std::ostream& os_;
    int indent_;

    void indent();
    void println(const char* s);
    void println(const std::string& s);

    class IndentGuard {
    public:
        IndentGuard(int& i) : indent_(i) { indent_ += 2; }
        ~IndentGuard() { indent_ -= 2; }
    private:
        int& indent_;
    };
};

} // namespace tiger

#endif // TIGER_AST_PRINTER_HPP
