#ifndef BLOCK_H
#define BLOCK_H

#include "tree/Stmt.h"

struct Block;
using block_ptr = std::shared_ptr<Block>;

struct Block : Stmt {
	StmtList stmts;

	Block(const StmtList & stmts) : Stmt(StmtType::Block), stmts(stmts) {}

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif