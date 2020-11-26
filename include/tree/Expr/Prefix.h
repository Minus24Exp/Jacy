#ifndef PREFIX_H
#define PREFIX_H

#include "tree/Expr/Expr.h"

namespace jc::tree {
    struct Prefix : Expr {
        parser::Token op;
        expr_ptr right;

        Prefix(const parser::Token & op, expr_ptr right)
                : Expr(op.pos, ExprType::Prefix), op(op), right(right) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif