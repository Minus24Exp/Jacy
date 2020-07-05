#ifndef WHILE_H
#define WHILE_H

#include "tree/Stmt.h"

struct While : Stmt {
	expr_ptr cond;
	block_ptr body;

	While(const Position & pos, expr_ptr cond, block_ptr body) : Stmt(pos, StmtType::While), cond(cond), body(body) {}
	virtual ~While() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif