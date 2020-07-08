#ifndef LITERAL_H
#define LITERAL_H

#include "tree/Expr/Expr.h"
#include "Token.h"

// I don't want to create many classes for all literal tokens
// so why do not just use one class that contains a token?

struct Literal : Expr {
	Token token;

	Literal(const Token & token) : Expr(token.pos, ExprType::Literal), token(token) {}
	virtual ~Literal() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif