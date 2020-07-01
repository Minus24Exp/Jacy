#ifndef NODE_H
#define NODE_H

#include "Token.h"
#include "BaseVisitor.h"

struct Node;
using ParseTree = std::vector<Node*>;

struct Statement;
using StatementList = std::vector<Statement*>;

struct Expression;
using ExpressionList = std::vector<Expression*>;

struct Node {
	Node() {}
	virtual ~Node() = default;

	virtual void accept(BaseVisitor & visitor) = 0;
};

struct Statement : Node {
	Statement() {}
	virtual ~Statement() = default;

	virtual void accept(BaseVisitor & visitor) = 0;
};

struct Expression : Node {
	Expression() {}
	virtual ~Expression() = default;

	virtual void accept(BaseVisitor & visitor) = 0;
};

struct ExprStmt : Statement {
	Expression & expr;

	ExprStmt(Expression & expr)
		: expr(expr) {}
	virtual ~ExprStmt() = default;

	virtual void accept(BaseVisitor & visitor) override {
		visitor.visit(*this);
	}
};

#endif