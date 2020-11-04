#ifndef LITERAL_H
#define LITERAL_H

#include "tree/Expr/Expr.h"
#include "parser/Token.h"

// I don't want to create many classes for all literal tokens
// so why do not just use one class that contains a token?

struct Literal : Expr {
    Token token;

    explicit Literal(const Token & token) : Expr(token.pos, ExprType::Literal), token(token) {}
    ~Literal() override = default;

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif