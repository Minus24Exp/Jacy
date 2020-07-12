#ifndef FUNCCALL_H
#define FUNCCALL_H

#include "tree/Expr/Expr.h"

struct FuncCall : Expr {
    expr_ptr left;
    ExprList args;

    FuncCall(const Position & pos, expr_ptr left, const ExprList & args)
            : Expr(pos, ExprType::Call), left(left), args(args) {}

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif