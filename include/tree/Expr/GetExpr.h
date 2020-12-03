#ifndef GETEXPR_H
#define GETEXPR_H

#include "tree/Expr/Identifier.h"

namespace jc::tree {
    struct GetExpr : public Expr {
        expr_ptr left;
        id_ptr id;
        bool nullish;

        GetExpr(expr_ptr left, bool nullish, id_ptr id)
            : Expr(left->pos, ExprType::Get), left(std::move(left)), nullish(nullish), id(std::move(id)) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif