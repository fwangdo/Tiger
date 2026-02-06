/**
 * Exercise 1: Basic AST Nodes - SOLUTION
 */

#include <iostream>
#include <string>
#include <memory>
#include <vector>

struct Position {
    int line, column;
    Position(int l = 1, int c = 1) : line(l), column(c) {}
};

// TODO 1: Define ExpKind enum
enum class ExpKind {
    INT, STRING, VAR, BINOP, CALL
};

// TODO 2: Define the base Exp struct
struct Exp {
    ExpKind kind;
    Position pos;

    Exp(ExpKind k, Position p) : kind(k), pos(p) {}
    virtual ~Exp() = default;
};

using ExpPtr = std::unique_ptr<Exp>;

// TODO 3: Define IntExp
struct IntExp : Exp {
    int value;

    IntExp(int v, Position p) : Exp(ExpKind::INT, p), value(v) {}
};

// TODO 4: Define StringExp
struct StringExp : Exp {
    std::string value;

    StringExp(const std::string& v, Position p)
        : Exp(ExpKind::STRING, p), value(v) {}
};

// TODO 5: Define VarExp
struct VarExp : Exp {
    std::string name;

    VarExp(const std::string& n, Position p)
        : Exp(ExpKind::VAR, p), name(n) {}
};

enum class Op { PLUS, MINUS, TIMES, DIVIDE };

// TODO 6: Define BinOpExp
struct BinOpExp : Exp {
    ExpPtr left;
    Op op;
    ExpPtr right;

    BinOpExp(ExpPtr l, Op o, ExpPtr r, Position p)
        : Exp(ExpKind::BINOP, p),
          left(std::move(l)), op(o), right(std::move(r)) {}
};

// TODO 7: Define CallExp
struct CallExp : Exp {
    std::string func;
    std::vector<ExpPtr> args;

    CallExp(const std::string& f, std::vector<ExpPtr> a, Position p)
        : Exp(ExpKind::CALL, p), func(f), args(std::move(a)) {}
};

const char* kind_to_string(ExpKind k) {
    switch (k) {
        case ExpKind::INT: return "INT";
        case ExpKind::STRING: return "STRING";
        case ExpKind::VAR: return "VAR";
        case ExpKind::BINOP: return "BINOP";
        case ExpKind::CALL: return "CALL";
        default: return "?";
    }
}

int main() {
    Position pos(1, 1);

    // Create: 1 + x
    ExpPtr one = std::make_unique<IntExp>(1, pos);
    ExpPtr x = std::make_unique<VarExp>("x", pos);
    ExpPtr add = std::make_unique<BinOpExp>(
        std::move(one), Op::PLUS, std::move(x), pos);

    // Create: print("hello")
    std::vector<ExpPtr> args;
    args.push_back(std::make_unique<StringExp>("hello", pos));
    ExpPtr call = std::make_unique<CallExp>("print", std::move(args), pos);

    std::cout << "Created expressions:" << std::endl;
    std::cout << "  add: " << kind_to_string(add->kind) << std::endl;
    std::cout << "  call: " << kind_to_string(call->kind) << std::endl;

    if (add->kind == ExpKind::BINOP && call->kind == ExpKind::CALL) {
        std::cout << "\n✓ AST nodes exercise complete!" << std::endl;
    } else {
        std::cout << "\n✗ Check your implementation" << std::endl;
    }

    return 0;
}
