#ifndef GETEXPR_H
#define GETEXPR_H

#include "tree/Expr/Expr.h"
#include "tree/Expr/Identifier.h"

namespace jc::tree {
    struct GetExpr : public Expr {
        expr_ptr left;
        id_ptr id;

        GetExpr(expr_ptr left, id_ptr id)
                : Expr(left->pos, ExprType::Get), left(left), id(id) {}
        virtual ~GetExpr() = default;

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif