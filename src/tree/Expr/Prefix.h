#ifndef PREFIX_H
#define PREFIX_H

#include "tree/Expr/Expr.h"

struct Prefix : Expr {
	Token op;
	expr_ptr right;

	Prefix(const Position & pos, const Token & op, expr_ptr right)
		: Expr(pos, ExprType::Prefix), op(op), right(right) {}
	virtual ~Prefix() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif