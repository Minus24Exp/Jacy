#ifndef FORSTMT_H
#define FORSTMT_H

#include "tree/Stmt/Stmt.h"

namespace jc::tree {
    struct ForStmt : Stmt {
        id_ptr For;
        expr_ptr In;
        block_ptr body;

        ForStmt(const Position & pos, id_ptr For, expr_ptr In, block_ptr body)
                : Stmt(pos, StmtType::For), For(For), In(In), body(body) {}
        virtual ~ForStmt() = default;

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif