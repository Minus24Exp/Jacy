#ifndef IFEXPRESSION_H
#define IFEXPRESSION_H

#include <vector>
#include "tree/Node.h"

struct ConditionStructure {
	expr_ptr cond;
	block_ptr body;
};

using ConditionList = std::vector<ConditionStructure>;

struct IfExpression : Expression {
	ConditionList conditions;
	block_ptr Else;

	IfExpression(const ConditionList & conditions, block_ptr Else)
		: conditions(conditions), Else(Else) {}
	virtual ~IfExpression() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif