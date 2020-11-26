#ifndef INFIX_H
#define INFIX_H

#include "tree/Expr/Expr.h"

namespace jc::tree {
    struct Infix : Expr {
        expr_ptr left;
        parser::Token op;
        expr_ptr right;

        Infix(expr_ptr left, const parser::Token & op, expr_ptr right)
                : Expr(left->pos, ExprType::Infix), left(left), op(op), right(right) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif