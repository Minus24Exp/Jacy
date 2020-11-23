#ifndef METHODCALL_H
#define METHODCALL_H

#include "tree/Expr/Identifier.h"

namespace jc::tree {
    struct MethodCall : Expr {
        expr_ptr left;
        id_ptr id;
        ExprList args;

        MethodCall(const Position & pos, expr_ptr left, id_ptr id, ExprList args)
            : Expr(pos, ExprType::MethodCall), left(left), id(id), args(std::move(args)) {}
        ~MethodCall() override = default;

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif // METHODCALL_H
