#ifndef WHILESTMT_H
#define WHILESTMT_H

#include "tree/Stmt/Stmt.h"

struct WhileStmt : Stmt {
    expr_ptr cond;
    block_ptr body;

    WhileStmt(const Position & pos, expr_ptr cond, block_ptr body) : Stmt(pos, StmtType::While), cond(cond), body(body) {}
    virtual ~WhileStmt() = default;

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif