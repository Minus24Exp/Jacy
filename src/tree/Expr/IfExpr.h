#ifndef IFEXPR_H
#define IFEXPR_H

#include "tree/Expr/Expr.h"
#include "tree/Stmt/Block.h"

struct IfExpr : Expr {
    expr_ptr cond;
    block_ptr if_branch;
    block_ptr else_branch;

    IfExpr(const Position & pos, expr_ptr cond, block_ptr if_branch, block_ptr else_branch)
        : Expr(pos, ExprType::If), cond(cond), if_branch(if_branch), else_branch(else_branch) {}
    virtual ~IfExpr() = default;

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif