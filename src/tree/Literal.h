#ifndef LITERAL_H
#define LITERAL_H

#include "tree/Node.h"
#include "Token.h"

// I don't want to create many classes for all literal tokens
// so why do not just use one class that contains a token?

struct Literal : Expression {
	Token token;

	Literal(const Token & token) : token(token) {}
	virtual ~Literal() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.visit(*this);
	}
};

#endif