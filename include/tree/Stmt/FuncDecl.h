#ifndef FUNCDECL_H
#define FUNCDECL_H

#include "tree/Expr/Identifier.h"
#include "tree/Stmt/Block.h"

struct FuncParam {
    id_ptr id;
    expr_ptr default_val;
};

using FuncParams = std::vector<FuncParam>;

enum class FuncMode {
    Raw,
    Set,
    Get
};

struct FuncDecl : Stmt {
    FuncMode mode;
    id_ptr id;
    FuncParams params;
    block_ptr body;

    FuncDecl(const Position & pos, FuncMode mode, id_ptr id, const FuncParams & params, block_ptr body)
        : Stmt(pos, StmtType::FuncDecl), mode(mode), id(id), params(params), body(body) {}

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif