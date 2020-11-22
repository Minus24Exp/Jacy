#ifndef FUNCCALL_H
#define FUNCCALL_H

#include <utility>

#include "tree/Expr/Expr.h"

namespace jc::tree {
    struct FuncCall : Expr {
        expr_ptr left;
        ExprList args;

        FuncCall(const expr_ptr & left, ExprList  args)
                : Expr(left->pos, ExprType::Call), left(left), args(std::move(args)) {}
        ~FuncCall() override = default;

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif