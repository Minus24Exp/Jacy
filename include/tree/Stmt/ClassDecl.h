#ifndef CLASSDECL_H
#define CLASSDECL_H

#include "tree/Stmt/Stmt.h"
#include "tree/Expr/Identifier.h"

struct ClassDecl : Stmt {
    id_ptr id;
    expr_ptr super;
    StmtList fields;

    ClassDecl(const Position & pos, id_ptr id, expr_ptr super, const StmtList & fields)
        : Stmt(pos, StmtType::Class), id(id), super(super), fields(fields) {}
    virtual ~ClassDecl() = default;

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif