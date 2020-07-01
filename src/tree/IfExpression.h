#ifndef IFEXPRESSION_H
#define IFEXPRESSION_H

#include <vector>
#include "tree/Node.h"

struct ConditionStructure {
	Expression & cond;
	Block & body;

	ConditionStructure(Expression & cond, Block & body)
		: cond(cond), body(body) {}
	virtual ~ConditionStructure() = default;
};

using ConditionList = std::vector<ConditionStructure>;

struct IfExpression : Expression {
	ConditionList conditions;
	Block * Else;

	IfExpression(const ConditionList & conditions, Block * Else)
		: conditions(conditions), Else(Else) {}
	virtual ~IfExpression() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.visit(*this);
	}
};

#endif