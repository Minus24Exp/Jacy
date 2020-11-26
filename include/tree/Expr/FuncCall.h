#ifndef FUNCCALL_H
#define FUNCCALL_H

#include <utility>

#include "tree/Expr/Expr.h"

namespace jc::tree {
    struct FuncArg {
        expr_ptr val;
        bool spread;
    };

    using FuncArgs = std::vector<FuncArg>;

    struct FuncCall : Expr {
        expr_ptr left;
        FuncArgs args;

        FuncCall(const expr_ptr & left, FuncArgs args)
            : Expr(left->pos, ExprType::Call), left(left), args(std::move(args)) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif