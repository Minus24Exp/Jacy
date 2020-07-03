#ifndef FUNCCALL_H
#define FUNCCALL_H

#include "tree/Node.h"

struct FuncCall : Expression {
	expr_ptr left;
	ExprList args;

	FuncCall(expr_ptr left, const ExprList & args) : left(left), args(args) {}

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif