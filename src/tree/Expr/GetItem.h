#ifndef GETITEM_H
#define GETITEM_H

#include "tree/Expr/Expr.h"

struct GetItem : Expr {
    expr_ptr left;
    expr_ptr index;

    GetItem(const Position & pos, expr_ptr left, expr_ptr index)
        : Expr(pos, ExprType::GetItem), left(left), index(index) {}
    virtual ~GetItem() = default;

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif