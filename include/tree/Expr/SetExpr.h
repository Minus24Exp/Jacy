#ifndef SETEXPR_H
#define SETEXPR_H

#include "tree/Expr/Expr.h"
#include "tree/Expr/Identifier.h"

struct SetExpr : public Expr {
    expr_ptr left;
    id_ptr id;
    expr_ptr value;

    SetExpr(expr_ptr left, id_ptr id, expr_ptr value)
        : Expr(left->pos, ExprType::Set), left(left), id(id), value(value) {}
    virtual ~SetExpr() = default;

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif