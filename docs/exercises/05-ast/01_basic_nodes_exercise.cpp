/**
 * Exercise 1: Basic AST Nodes
 *
 * Design AST node types for a simple expression language.
 *
 * Compile: g++ -std=c++17 -o test 01_basic_nodes_exercise.cpp
 * Run: ./test
 */

#include <iostream>
#include <string>
#include <memory>
#include <vector>

// Position tracks source location
struct Position {
    int line, column;
    Position(int l = 1, int c = 1) : line(l), column(c) {}
};

// TODO 1: Define ExpKind enum
// Include: INT, STRING, VAR, BINOP, CALL
enum class ExpKind {
    ___
};

// TODO 2: Define the base Exp struct
// Fields: kind (ExpKind), pos (Position)
// Include virtual destructor
struct Exp {
    ___

    Exp(ExpKind k, Position p) : ___ {}
    virtual ~Exp() = default;
};

using ExpPtr = std::unique_ptr<Exp>;

// TODO 3: Define IntExp (integer literal)
// Field: value (int)
struct IntExp : Exp {
    ___

    IntExp(int v, Position p) : ___ {}
};

// TODO 4: Define StringExp (string literal)
// Field: value (std::string)
struct StringExp : Exp {
    ___

    StringExp(const std::string& v, Position p) : ___ {}
};

// TODO 5: Define VarExp (variable reference)
// Field: name (std::string)
struct VarExp : Exp {
    ___

    VarExp(const std::string& n, Position p) : ___ {}
};

// Binary operator enum
enum class Op { PLUS, MINUS, TIMES, DIVIDE };

// TODO 6: Define BinOpExp (binary operation)
// Fields: left (ExpPtr), op (Op), right (ExpPtr)
struct BinOpExp : Exp {
    ___

    BinOpExp(ExpPtr l, Op o, ExpPtr r, Position p)
        : ___ {}
};

// TODO 7: Define CallExp (function call)
// Fields: func (std::string), args (std::vector<ExpPtr>)
struct CallExp : Exp {
    ___

    CallExp(const std::string& f, std::vector<ExpPtr> a, Position p)
        : ___ {}
};

// Helper to print expression kind
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

    // Verify types
    if (add->kind == ExpKind::BINOP && call->kind == ExpKind::CALL) {
        std::cout << "\n✓ AST nodes exercise complete!" << std::endl;
    } else {
        std::cout << "\n✗ Check your implementation" << std::endl;
    }

    return 0;
}
