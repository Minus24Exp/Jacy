#ifndef IFEXPR_H
#define IFEXPR_H

#include <vector>
#include "tree/Expr.h"

struct ConditionStructure {
	expr_ptr cond;
	block_ptr body;
};

using ConditionList = std::vector<ConditionStructure>;

struct IfExpr : Expr {
	ConditionList conditions;
	block_ptr Else;

	IfExpr(const ConditionList & conditions, block_ptr Else)
		: Expr(ExprType::If), conditions(conditions), Else(Else) {}
	virtual ~IfExpr() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif