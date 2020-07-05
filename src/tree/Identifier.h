#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "tree/Expr.h"
#include <iostream>

struct Identifier;
using id_ptr = std::shared_ptr<Identifier>;

struct Identifier : Expr {
	Token token;

	Identifier(const Token & token) : Expr(token.pos, ExprType::Id), token(token) {}

	std::string get_name(){
		return token.String();
	}

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif