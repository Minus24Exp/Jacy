#ifndef DICTEXPR_H
#define DICTEXPR_H

#include "tree/Expr/Expr.h"
#include "tree/Expr/Identifier.h"

namespace jc::tree {
    struct DictElement {
        id_ptr id_key;
        expr_ptr expr_key;
        expr_ptr val;
    };

    using DictElementList = std::vector<DictElement>;

    struct DictExpr : Expr {
        DictElementList elements;

        DictExpr(const Position & pos, const DictElementList & elements) : Expr(pos, ExprType::Dict), elements(elements) {}
        virtual ~DictExpr() = default;

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif