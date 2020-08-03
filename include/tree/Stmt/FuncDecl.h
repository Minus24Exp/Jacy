#ifndef FUNCDECL_H
#define FUNCDECL_H

#include "tree/Expr/Identifier.h"
#include "tree/Stmt/Block.h"

struct FuncParam {
    id_ptr id;
    expr_ptr default_val;
};

using FuncParams = std::vector<FuncParam>;

struct FuncDecl : Stmt {
    id_ptr id;
    FuncParams params;
    block_ptr body;

    FuncDecl(const Position & pos, id_ptr id, const FuncParams & params, block_ptr body)
        : Stmt(pos, StmtType::FuncDecl), id(id), params(params), body(body) {}

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif