#ifndef GETEXPR_H
#define GETEXPR_H

#include "tree/Expr/Expr.h"
#include "tree/Expr/Identifier.h"

struct GetExpr : public Expr {
	expr_ptr left;
	id_ptr member;

	GetExpr(const Position & pos, expr_ptr left, id_ptr member)
		: Expr(pos, ExprType::Get), left(left), member(member) {}
	virtual ~GetExpr() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif