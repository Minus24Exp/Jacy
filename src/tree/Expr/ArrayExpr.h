#ifndef ARRAYEXPR_H
#define ARRAYEXPR_H

#include "tree/Expr/Expr.h"

/**
 * TODO: Think about alternative for ArrayExpr, it sounds pretty strange
 */

struct ArrayExpr : Expr {
    ExprList elements;

    ArrayExpr(const Position & pos, const ExprList & elements)
        : Expr(pos, ExprType::Array), elements(elements) {}
    virtual ~ArrayExpr() = default;

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif