#ifndef RETURNSTMT_H
#define RETURNSTMT_H

#include "tree/Stmt.h"

struct ReturnStmt : Stmt {
	expr_ptr expr;
	ReturnStmt(const Position & pos, expr_ptr expr) : Stmt(pos, StmtType::Return), expr(expr) {}
	virtual ~ReturnStmt() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif