# AST Design Exercises

Learn to design and implement Abstract Syntax Tree structures.

## Topics Covered

1. Node type hierarchy
2. Using smart pointers
3. Visitor pattern
4. AST printing/traversal

## Exercises

| File | Topic | Difficulty |
|------|-------|------------|
| `01_basic_nodes` | Define expression nodes | Easy |
| `02_smart_pointers` | Using unique_ptr | Easy |
| `03_visitor` | Visitor pattern | Medium |
| `04_ast_printer` | Tree printing | Medium |

## Key Concepts

### Node Hierarchy Pattern
```cpp
enum class ExprKind { INT, BINOP, VAR, CALL };

struct Expr {
    ExprKind kind;
    Position pos;
    virtual ~Expr() = default;
};

struct IntExpr : Expr {
    int value;
    IntExpr(int v, Position p) : Expr{ExprKind::INT, p}, value(v) {}
};
```

### Smart Pointers
```cpp
using ExprPtr = std::unique_ptr<Expr>;

// Creating nodes
ExprPtr node = std::make_unique<IntExpr>(42, pos);

// Moving ownership
ExprPtr left = std::make_unique<IntExpr>(1, pos);
ExprPtr right = std::make_unique<IntExpr>(2, pos);
ExprPtr binop = std::make_unique<BinOpExpr>(
    std::move(left), '+', std::move(right), pos);
```

### Visitor Pattern
```cpp
struct Visitor {
    virtual void visit(IntExpr& e) = 0;
    virtual void visit(BinOpExpr& e) = 0;
};

struct Expr {
    virtual void accept(Visitor& v) = 0;
};

struct IntExpr : Expr {
    void accept(Visitor& v) override { v.visit(*this); }
};
```
