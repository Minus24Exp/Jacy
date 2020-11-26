#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "tree/Expr/Expr.h"
#include <iostream>

namespace jc::tree {
    struct Identifier;
    using id_ptr = std::shared_ptr<Identifier>;

    struct Identifier : Expr {
        parser::Token token;

        explicit Identifier(const parser::Token & token) : Expr(token.pos, ExprType::Id), token(token) {}

        std::string get_name() const {
            return token.val;
        }

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif