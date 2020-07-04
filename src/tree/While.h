#ifndef WHILE_H
#define WHILE_H

#include "tree/Stmt.h"

struct While : Stmt {
	expr_ptr cond;
	block_ptr body;

	While(expr_ptr cond, block_ptr body) : Stmt(StmtType::While), cond(cond), body(body) {}
	virtual ~While() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif