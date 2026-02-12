#include "ASTPrinter.hpp"

namespace tiger {

void AstPrinter::indent() {
    for (int i = 0; i < indent_; i++) {
        os_ << ' ';
    }
}

void AstPrinter::println(const char* s) {
    indent();
    os_ << s << "\n";
}

void AstPrinter::println(const std::string& s) {
    indent();
    os_ << s << "\n";
}

void AstPrinter::print(const Program& prog) {
    println("Program");
    IndentGuard g(indent_);
    if (prog.exp) {
        print(*prog.exp);
    }
}

static const char* op_to_string(Op op) {
    switch (op) {
        case Op::PLUS:   return "+";
        case Op::MINUS:  return "-";
        case Op::TIMES:  return "*";
        case Op::DIVIDE: return "/";
        case Op::EQ:     return "=";
        case Op::NEQ:    return "<>";
        case Op::LT:     return "<";
        case Op::LE:     return "<=";
        case Op::GT:     return ">";
        case Op::GE:     return ">=";
    }
    return "?";
}

void AstPrinter::print(const Exp& exp) {
    switch (exp.kind) {
        case ExpKind::VAR: {
            const auto& e = static_cast<const VarExp&>(exp);
            println("VarExp");
            IndentGuard g(indent_);
            print(*e.var);
            break;
        }
        case ExpKind::NIL:
            println("NilExp");
            break;
        case ExpKind::INT: {
            const auto& e = static_cast<const IntExp&>(exp);
            println("IntExp: " + std::to_string(e.value));
            break;
        }
        case ExpKind::STRING: {
            const auto& e = static_cast<const StringExp&>(exp);
            println("StringExp: \"" + e.value + "\"");
            break;
        }
        case ExpKind::CALL: {
            const auto& e = static_cast<const CallExp&>(exp);
            println("CallExp: " + e.func);
            IndentGuard g(indent_);
            for (const auto& arg : e.args) {
                print(*arg);
            }
            break;
        }
        case ExpKind::OP: {
            const auto& e = static_cast<const OpExp&>(exp);
            println(std::string("OpExp: ") + op_to_string(e.op));
            IndentGuard g(indent_);
            print(*e.left);
            print(*e.right);
            break;
        }
        case ExpKind::RECORD: {
            const auto& e = static_cast<const RecordExp&>(exp);
            println("RecordExp: " + e.type_id);
            IndentGuard g(indent_);
            for (const auto& f : e.fields) {
                println("field: " + f.name);
                IndentGuard g2(indent_);
                print(*f.exp);
            }
            break;
        }
        case ExpKind::SEQ: {
            const auto& e = static_cast<const SeqExp&>(exp);
            println("SeqExp");
            IndentGuard g(indent_);
            for (const auto& ex : e.exps) {
                print(*ex);
            }
            break;
        }
        case ExpKind::ASSIGN: {
            const auto& e = static_cast<const AssignExp&>(exp);
            println("AssignExp");
            IndentGuard g(indent_);
            print(*e.var);
            print(*e.exp);
            break;
        }
        case ExpKind::IF: {
            const auto& e = static_cast<const IfExp&>(exp);
            println("IfExp");
            IndentGuard g(indent_);
            println("test:");
            { IndentGuard g2(indent_); print(*e.test); }
            println("then:");
            { IndentGuard g2(indent_); print(*e.then_exp); }
            if (e.else_exp) {
                println("else:");
                IndentGuard g2(indent_);
                print(*e.else_exp);
            }
            break;
        }
        case ExpKind::WHILE: {
            const auto& e = static_cast<const WhileExp&>(exp);
            println("WhileExp");
            IndentGuard g(indent_);
            println("test:");
            { IndentGuard g2(indent_); print(*e.test); }
            println("body:");
            { IndentGuard g2(indent_); print(*e.body); }
            break;
        }
        case ExpKind::FOR: {
            const auto& e = static_cast<const ForExp&>(exp);
            println("ForExp: " + e.var);
            IndentGuard g(indent_);
            println("lo:");
            { IndentGuard g2(indent_); print(*e.lo); }
            println("hi:");
            { IndentGuard g2(indent_); print(*e.hi); }
            println("body:");
            { IndentGuard g2(indent_); print(*e.body); }
            break;
        }
        case ExpKind::BREAK:
            println("BreakExp");
            break;
        case ExpKind::LET: {
            const auto& e = static_cast<const LetExp&>(exp);
            println("LetExp");
            IndentGuard g(indent_);
            println("decs:");
            for (const auto& d : e.decs) {
                IndentGuard g2(indent_);
                print(*d);
            }
            println("body:");
            for (const auto& b : e.body) {
                IndentGuard g2(indent_);
                print(*b);
            }
            break;
        }
        case ExpKind::ARRAY: {
            const auto& e = static_cast<const ArrayExp&>(exp);
            println("ArrayExp: " + e.type_id);
            IndentGuard g(indent_);
            println("size:");
            { IndentGuard g2(indent_); print(*e.size); }
            println("init:");
            { IndentGuard g2(indent_); print(*e.init); }
            break;
        }
    }
}

void AstPrinter::print(const Var& var) {
    switch (var.kind) {
        case VarKind::SIMPLE: {
            const auto& v = static_cast<const SimpleVar&>(var);
            println("SimpleVar: " + v.name);
            break;
        }
        case VarKind::FIELD: {
            const auto& v = static_cast<const FieldVar&>(var);
            println("FieldVar: ." + v.field);
            IndentGuard g(indent_);
            print(*v.var);
            break;
        }
        case VarKind::SUBSCRIPT: {
            const auto& v = static_cast<const SubscriptVar&>(var);
            println("SubscriptVar");
            IndentGuard g(indent_);
            print(*v.var);
            println("index:");
            { IndentGuard g2(indent_); print(*v.index); }
            break;
        }
    }
}

void AstPrinter::print(const Dec& dec) {
    switch (dec.kind) {
        case DecKind::VAR: {
            const auto& d = static_cast<const VarDec&>(dec);
            std::string type_str = d.type_id.empty() ? "" : " : " + d.type_id;
            println("VarDec: " + d.name + type_str);
            IndentGuard g(indent_);
            print(*d.init);
            break;
        }
        case DecKind::TYPE: {
            const auto& d = static_cast<const TypeDec&>(dec);
            println("TypeDec: " + d.name);
            IndentGuard g(indent_);
            print(*d.ty);
            break;
        }
        case DecKind::FUNCTION: {
            const auto& d = static_cast<const FunctionDec&>(dec);
            std::string ret = d.result_type.empty() ? "" : " : " + d.result_type;
            println("FunctionDec: " + d.name + ret);
            IndentGuard g(indent_);
            if (!d.params.empty()) {
                println("params:");
                IndentGuard g2(indent_);
                for (const auto& p : d.params) {
                    println(p.name + " : " + p.type_id);
                }
            }
            println("body:");
            { IndentGuard g2(indent_); print(*d.body); }
            break;
        }
    }
}

void AstPrinter::print(const Ty& ty) {
    switch (ty.kind) {
        case TyKind::NAME: {
            const auto& t = static_cast<const NameTy&>(ty);
            println("NameTy: " + t.name);
            break;
        }
        case TyKind::RECORD: {
            const auto& t = static_cast<const RecordTy&>(ty);
            println("RecordTy");
            IndentGuard g(indent_);
            for (const auto& f : t.fields) {
                println(f.name + " : " + f.type_id);
            }
            break;
        }
        case TyKind::ARRAY: {
            const auto& t = static_cast<const ArrayTy&>(ty);
            println("ArrayTy: array of " + t.element_type);
            break;
        }
    }
}

} // namespace tiger
