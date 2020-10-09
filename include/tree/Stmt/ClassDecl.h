#ifndef CLASSDECL_H
#define CLASSDECL_H

#include <utility>

#include "tree/Stmt/Stmt.h"
#include "tree/Expr/Identifier.h"

struct ClassDecl : Stmt {
    id_ptr id;
    expr_ptr super;
    StmtList fields;

    ClassDecl(const Position & pos, id_ptr id, expr_ptr super, StmtList  fields)
        : Stmt(pos, StmtType::Class), id(std::move(id)), super(std::move(super)), fields(std::move(fields)) {}
    ~ClassDecl() override = default;

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif