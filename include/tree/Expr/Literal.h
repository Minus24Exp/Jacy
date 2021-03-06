#ifndef LITERAL_H
#define LITERAL_H

#include "tree/Expr/Expr.h"
#include "parser/Token.h"

namespace jc::tree {
    // I don't want to create many classes for all literal tokens
    // so why do not just use one class that contains a token?

    struct Literal : Expr {
        parser::Token token;

        explicit Literal(const parser::Token & token) : Expr(token.pos, ExprType::Literal), token(token) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif