#ifndef NODE_H
#define NODE_H

#include <memory>
#include "Token.h"
#include "BaseVisitor.h"

struct Statement;
using stmt_ptr = std::shared_ptr<Statement>;
using StmtList = std::vector<stmt_ptr>;

struct Expression;
using expr_ptr = std::shared_ptr<Expression>;
using ExprList = std::vector<expr_ptr>;

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
	expr_ptr expr;

	ExprStmt(expr_ptr expr)
		: expr(expr) {}
	virtual ~ExprStmt() = default;

	virtual void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif