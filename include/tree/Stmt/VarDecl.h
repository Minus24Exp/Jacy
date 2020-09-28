#ifndef VARDECL_H
#define VARDECL_H

#include "tree/Expr/Identifier.h"
#include "tree/Stmt/Stmt.h"

// Note: I used enum for declaration type, but not a bool
// because I want to add `const` in the future

// @TODO: Add types

enum class VarDeclKind { Var, Val };

struct VarDecl : Stmt {
    VarDeclKind kind;
    id_ptr id;
    expr_ptr assign_expr;

    VarDecl(const Position & pos, const VarDeclKind & kind, id_ptr id, expr_ptr assign_expr)
        : Stmt(pos, StmtType::VarDecl), kind(kind), id(id), assign_expr(assign_expr) {}

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif