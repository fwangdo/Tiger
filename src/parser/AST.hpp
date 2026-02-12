#ifndef TIGER_AST_HPP
#define TIGER_AST_HPP

#include "lexer/Token.hpp"
#include <memory>
#include <string>
#include <vector>

namespace tiger {

// Forward declarations
struct Exp;
struct Var;
struct Dec;
struct Ty;

using ExpPtr = std::unique_ptr<Exp>;
using VarPtr = std::unique_ptr<Var>;
using DecPtr = std::unique_ptr<Dec>;
using TyPtr = std::unique_ptr<Ty>;

// ============================================================================
// Expressions
// ============================================================================

enum class ExpKind {
    VAR,
    NIL,
    INT,
    STRING,
    CALL,
    OP,
    RECORD,
    SEQ,
    ASSIGN,
    IF,
    WHILE,
    FOR,
    BREAK,
    LET,
    ARRAY,
};

struct Field {
    std::string name;
    ExpPtr exp;
    Position pos;

    Field(const std::string& n, ExpPtr e, Position p)
        : name(n), exp(std::move(e)), pos(p) {}
};

struct Exp {
    ExpKind kind;
    Position pos;

    explicit Exp(ExpKind k, Position p) : kind(k), pos(p) {}
    virtual ~Exp() = default;
};

struct VarExp : Exp {
    VarPtr var;

    VarExp(VarPtr v, Position p)
        : Exp(ExpKind::VAR, p), var(std::move(v)) {}
};

struct NilExp : Exp {
    explicit NilExp(Position p) : Exp(ExpKind::NIL, p) {}
};

struct IntExp : Exp {
    int value;

    IntExp(int v, Position p) : Exp(ExpKind::INT, p), value(v) {}
};

struct StringExp : Exp {
    std::string value;

    StringExp(const std::string& v, Position p)
        : Exp(ExpKind::STRING, p), value(v) {}
};

struct CallExp : Exp {
    std::string func;
    std::vector<ExpPtr> args;

    CallExp(const std::string& f, std::vector<ExpPtr> a, Position p)
        : Exp(ExpKind::CALL, p), func(f), args(std::move(a)) {}
};

enum class Op {
    PLUS, MINUS, TIMES, DIVIDE,
    EQ, NEQ, LT, LE, GT, GE,
};

struct OpExp : Exp {
    ExpPtr left;
    Op op;
    ExpPtr right;

    OpExp(ExpPtr l, Op o, ExpPtr r, Position p)
        : Exp(ExpKind::OP, p), left(std::move(l)), op(o), right(std::move(r)) {}
};

struct RecordExp : Exp {
    std::string type_id;
    std::vector<Field> fields;

    RecordExp(const std::string& t, std::vector<Field> f, Position p)
        : Exp(ExpKind::RECORD, p), type_id(t), fields(std::move(f)) {}
};

struct SeqExp : Exp {
    std::vector<ExpPtr> exps;

    SeqExp(std::vector<ExpPtr> e, Position p)
        : Exp(ExpKind::SEQ, p), exps(std::move(e)) {}
};

struct AssignExp : Exp {
    VarPtr var;
    ExpPtr exp;

    AssignExp(VarPtr v, ExpPtr e, Position p)
        : Exp(ExpKind::ASSIGN, p), var(std::move(v)), exp(std::move(e)) {}
};

struct IfExp : Exp {
    ExpPtr test;
    ExpPtr then_exp;
    ExpPtr else_exp;  // nullptr if no else

    IfExp(ExpPtr t, ExpPtr th, ExpPtr el, Position p)
        : Exp(ExpKind::IF, p), test(std::move(t)),
          then_exp(std::move(th)), else_exp(std::move(el)) {}
};

struct WhileExp : Exp {
    ExpPtr test;
    ExpPtr body;

    WhileExp(ExpPtr t, ExpPtr b, Position p)
        : Exp(ExpKind::WHILE, p), test(std::move(t)), body(std::move(b)) {}
};

struct ForExp : Exp {
    std::string var;
    ExpPtr lo;
    ExpPtr hi;
    ExpPtr body;

    ForExp(const std::string& v, ExpPtr l, ExpPtr h, ExpPtr b, Position p)
        : Exp(ExpKind::FOR, p), var(v),
          lo(std::move(l)), hi(std::move(h)), body(std::move(b)) {}
};

struct BreakExp : Exp {
    explicit BreakExp(Position p) : Exp(ExpKind::BREAK, p) {}
};

struct LetExp : Exp {
    std::vector<DecPtr> decs;
    std::vector<ExpPtr> body;

    LetExp(std::vector<DecPtr> d, std::vector<ExpPtr> b, Position p)
        : Exp(ExpKind::LET, p), decs(std::move(d)), body(std::move(b)) {}
};

struct ArrayExp : Exp {
    std::string type_id;
    ExpPtr size;
    ExpPtr init;

    ArrayExp(const std::string& t, ExpPtr s, ExpPtr i, Position p)
        : Exp(ExpKind::ARRAY, p), type_id(t),
          size(std::move(s)), init(std::move(i)) {}
};

// ============================================================================
// Variables (L-values)
// ============================================================================

enum class VarKind {
    SIMPLE,
    FIELD,
    SUBSCRIPT,
};

struct Var {
    VarKind kind;
    Position pos;

    explicit Var(VarKind k, Position p) : kind(k), pos(p) {}
    virtual ~Var() = default;
};

struct SimpleVar : Var {
    std::string name;

    SimpleVar(const std::string& n, Position p)
        : Var(VarKind::SIMPLE, p), name(n) {}
};

struct FieldVar : Var {
    VarPtr var;
    std::string field;

    FieldVar(VarPtr v, const std::string& f, Position p)
        : Var(VarKind::FIELD, p), var(std::move(v)), field(f) {}
};

struct SubscriptVar : Var {
    VarPtr var;
    ExpPtr index;

    SubscriptVar(VarPtr v, ExpPtr i, Position p)
        : Var(VarKind::SUBSCRIPT, p), var(std::move(v)), index(std::move(i)) {}
};

// ============================================================================
// Declarations
// ============================================================================

enum class DecKind {
    VAR,
    TYPE,
    FUNCTION,
};

struct TypeField {
    std::string name;
    std::string type_id;
    Position pos;

    TypeField(const std::string& n, const std::string& t, Position p)
        : name(n), type_id(t), pos(p) {}
};

struct Dec {
    DecKind kind;
    Position pos;

    explicit Dec(DecKind k, Position p) : kind(k), pos(p) {}
    virtual ~Dec() = default;
};

struct VarDec : Dec {
    std::string name;
    std::string type_id;  // empty if not specified
    ExpPtr init;

    VarDec(const std::string& n, const std::string& t, ExpPtr i, Position p)
        : Dec(DecKind::VAR, p), name(n), type_id(t), init(std::move(i)) {}
};

struct TypeDec : Dec {
    std::string name;
    TyPtr ty;

    TypeDec(const std::string& n, TyPtr t, Position p)
        : Dec(DecKind::TYPE, p), name(n), ty(std::move(t)) {}
};

struct FunctionDec : Dec {
    std::string name;
    std::vector<TypeField> params;
    std::string result_type;  // empty if void
    ExpPtr body;

    FunctionDec(const std::string& n, std::vector<TypeField> p,
                const std::string& r, ExpPtr b, Position pos)
        : Dec(DecKind::FUNCTION, pos), name(n), params(std::move(p)),
          result_type(r), body(std::move(b)) {}
};

// ============================================================================
// Types
// ============================================================================

enum class TyKind {
    NAME,
    RECORD,
    ARRAY,
};

struct Ty {
    TyKind kind;
    Position pos;

    explicit Ty(TyKind k, Position p) : kind(k), pos(p) {}
    virtual ~Ty() = default;
};

struct NameTy : Ty {
    std::string name;

    NameTy(const std::string& n, Position p)
        : Ty(TyKind::NAME, p), name(n) {}
};

struct RecordTy : Ty {
    std::vector<TypeField> fields;

    RecordTy(std::vector<TypeField> f, Position p)
        : Ty(TyKind::RECORD, p), fields(std::move(f)) {}
};

struct ArrayTy : Ty {
    std::string element_type;

    ArrayTy(const std::string& e, Position p)
        : Ty(TyKind::ARRAY, p), element_type(e) {}
};

// ============================================================================
// Program
// ============================================================================

struct Program {
    ExpPtr exp;
    Position pos;

    Program(ExpPtr e, Position p) : exp(std::move(e)), pos(p) {}
};

} // namespace tiger

#endif // TIGER_AST_HPP
