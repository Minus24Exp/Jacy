#ifndef BLOCK_H
#define BLOCK_H

#include "tree/Node.h"

struct Block;
using block_ptr = std::shared_ptr<Block>;

struct Block : Statement {
	StmtList stmts;

	Block(const StmtList & stmts) : stmts(stmts) {}

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif