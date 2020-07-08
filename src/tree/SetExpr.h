#ifndef SETEXPR_H
#define SETEXPR_H

#include "tree/Expr.h"

class SetExpr : public Expr {
	expr_ptr left;
	id_ptr member;
	expr_ptr value;

	SetExpr(expr_ptr object, id_ptr member, expr_ptr value)
		: object(object), member(member), value(value) {}
	virtual ~SetExpr() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.accept(this);
	}
};

#endif