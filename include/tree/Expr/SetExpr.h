#ifndef SETEXPR_H
#define SETEXPR_H

#include "tree/Expr/Expr.h"

#include <utility>
#include "tree/Expr/Identifier.h"

namespace jc::tree {
    struct SetExpr : public Expr {
        expr_ptr left;
        id_ptr id;
        parser::Token assign_op;
        expr_ptr value;

        SetExpr(const expr_ptr & left, id_ptr id, parser::Token assign_op, expr_ptr value)
            : Expr(left->pos, ExprType::Set), left(left), id(std::move(id)), assign_op(std::move(assign_op)), value(std::move(value)) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif