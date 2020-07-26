#ifndef LISTEXPR_H
#define LISTEXPR_H

#include "tree/Expr/Expr.h"

/**
 * TODO: Think about alternative for ListExpr, it sounds pretty strange
 */

struct ListExpr : Expr {
    ExprList elements;

    ListExpr(const Position & pos, const ExprList & elements)
        : Expr(pos, ExprType::List), elements(elements) {}
    virtual ~ListExpr() = default;

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif