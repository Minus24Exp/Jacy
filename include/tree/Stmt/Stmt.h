#ifndef STMT_H
#define STMT_H

#include "tree/Node.h"
#include "tree/Expr/Expr.h"

struct Stmt;
using stmt_ptr = std::shared_ptr<Stmt>;
using StmtList = std::vector<stmt_ptr>;

enum class StmtType {
    Expr, // Expression Statement
    Block,
    FuncDecl,
    VarDecl,
    While,
    Return,
    Class,
    Import,
    For,
    Type
};

struct Stmt : Node {
    Stmt(const Position & pos, StmtType type) : Node(pos), type(type) {}
    ~Stmt() override = default;

    void accept(BaseVisitor & visitor) override = 0;

    StmtType type;
};

struct ExprStmt : Stmt {
    expr_ptr expr;

    explicit ExprStmt(expr_ptr expr) : Stmt(expr->pos, StmtType::Expr), expr(expr) {}
    ~ExprStmt() override = default;

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif