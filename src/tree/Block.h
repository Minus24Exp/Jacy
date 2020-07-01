#ifndef BLOCK_H
#define BLOCK_H

#include "tree/Node.h"

struct Block : Expression {
	StatementList stmts;

	Block(const StatementList & stmts) : stmts(stmts) {}

	void accept(BaseVisitor & visitor) override {
		visitor.visit(*this);
	}
};

#endif