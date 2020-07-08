#ifndef SETEXPR_H
#define SETEXPR_H

#include "tree/Expr/Expr.h"
#include "tree/Expr/Identifier.h"

struct SetExpr : public Expr {
	expr_ptr left;
	id_ptr member;
	expr_ptr value;

	SetExpr(const Position & pos, expr_ptr left, id_ptr member, expr_ptr value)
		: Expr(pos, ExprType::Set), left(left), member(member), value(value) {}
	virtual ~SetExpr() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif