#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "tree/Expr/Expr.h"
#include <iostream>

struct Identifier;
using id_ptr = std::shared_ptr<Identifier>;

struct Identifier : Expr {
    Token token;

    explicit Identifier(const Token & token) : Expr(token.pos, ExprType::Id), token(token) {}
    ~Identifier() override = default;

    std::string get_name() const {
        return token.val;
    }

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif