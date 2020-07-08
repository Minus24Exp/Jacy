#ifndef INFIX_H
#define INFIX_H

#include "tree/Expr/Expr.h"

struct Infix : Expr {
	expr_ptr left;
	Token op;
	expr_ptr right;

	Infix(const Position & pos, expr_ptr left, const Token & op, expr_ptr right)
		   : Expr(pos, ExprType::Infix), left(left), op(op), right(right) {}

	virtual ~Infix() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif