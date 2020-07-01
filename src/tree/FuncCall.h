#ifndef FUNCCALL_H
#define FUNCCALL_H

#include "tree/Node.h"

struct FuncCall : Expression {
	Expression & left;
	ExpressionList args;

	FuncCall(Expression & left, const ExpressionList & args)
		: left(left), args(args) {}

	void accept(BaseVisitor & visitor) override {
		visitor.visit(*this);
	}
};

#endif