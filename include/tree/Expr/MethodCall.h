#ifndef METHODCALL_H
#define METHODCALL_H

#include "tree/Expr/Identifier.h"
#include "tree/Expr/FuncCall.h"

namespace jc::tree {
    struct MethodCall : Expr {
        expr_ptr left;
        id_ptr id;
        FuncArgs args;

        MethodCall(expr_ptr left, id_ptr id, FuncArgs args)
            : Expr(left->pos, ExprType::MethodCall), left(std::move(left)), id(std::move(id)), args(std::move(args)) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif // METHODCALL_H
