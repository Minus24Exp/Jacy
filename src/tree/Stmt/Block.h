#ifndef BLOCK_H
#define BLOCK_H

#include "tree/Stmt/Stmt.h"

struct Block;
using block_ptr = std::shared_ptr<Block>;

struct Block : Stmt {
	StmtList stmts;

	Block(const Position & pos, const StmtList & stmts) : Stmt(pos, StmtType::Block), stmts(stmts) {}

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif