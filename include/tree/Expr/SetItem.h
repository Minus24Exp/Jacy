#ifndef SETITEM_H
#define SETITEM_H

#include <utility>

#include "tree/Expr/Expr.h"

namespace jc::tree {
    struct SetItem : Expr {
        expr_ptr left;
        expr_ptr index;
        parser::Token assign_op;
        expr_ptr value;

        SetItem(expr_ptr left, expr_ptr index, parser::Token assign_op, expr_ptr value)
            : Expr(left->pos, ExprType::SetItem),
              left(std::move(left)),
              index(std::move(index)),
              assign_op(std::move(assign_op)),
              value(std::move(value)) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif