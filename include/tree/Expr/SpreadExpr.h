#ifndef SPREADEXPR_H
#define SPREADEXPR_H

#include "tree/Expr/Expr.h"

namespace jc::tree {
    struct SpreadExpr : Expr {
        expr_ptr expr;

        SpreadExpr(const Position & pos, expr_ptr expr) : Expr(pos, ExprType::Spread), expr(std::move(expr)) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(*this);
        }
    };
}

#endif // SPREADEXPR_H
