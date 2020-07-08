#ifndef GETEXPR_H
#define GETEXPR_H

#include "tree/Expr.h"

struct GetExpr : public Expr {
	expr_ptr left;
	id_ptr member;

	GetExpr(expr_ptr left, id_ptr member) : Expr(ExprType::Get) {}
	virtual ~GetExpr() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.accept(this);
	}
};

#endif