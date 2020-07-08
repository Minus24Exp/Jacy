#ifndef POSTFIX_H
#define POSTFIX_H

#include "tree/Expr/Expr.h"

struct Postfix : Expr {
	expr_ptr left;
	Token op;

	Postfix(const Position & pos, expr_ptr left, const Token & op)
		: Expr(pos, ExprType::Postfix), left(left), op(op) {}
	virtual ~Postfix() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif