#ifndef SETEXPR_H
#define SETEXPR_H

#include "tree/Expr/Expr.h"

#include <utility>
#include "tree/Expr/Identifier.h"

struct SetExpr : public Expr {
    expr_ptr left;
    id_ptr id;
    expr_ptr value;

    SetExpr(const expr_ptr& left, id_ptr id, expr_ptr value)
        : Expr(left->pos, ExprType::Set), left(left), id(std::move(id)), value(std::move(value)) {}
    ~SetExpr() override = default;

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif