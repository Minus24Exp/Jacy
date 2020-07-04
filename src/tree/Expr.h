#ifndef EXPR_H
#define EXPR_H

#include "tree/Node.h"

struct Expr;
using expr_ptr = std::shared_ptr<Expr>;
using ExprList = std::vector<expr_ptr>;

enum class ExprType {
	Id,
	Call,
	If,
	Infix,
	Literal
};

struct Expr : Node {
	Expr(ExprType type) : type(type) {}
	virtual ~Expr() = default;

	virtual void accept(BaseVisitor & visitor) = 0;

	ExprType type;
};

#endif