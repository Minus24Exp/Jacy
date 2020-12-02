#ifndef GROUPING_H
#define GROUPING_H

#include "tree/Expr/Expr.h"

namespace jc::tree {
    struct Grouping : Expr {
        Grouping(const Position & pos, expr_ptr expr) : Expr(pos, ExprType::Grouping), expr(std::move(expr)) {}

        expr_ptr expr;

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif // GROUPING_H
