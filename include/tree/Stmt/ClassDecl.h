#ifndef CLASSDECL_H
#define CLASSDECL_H

#include "tree/Stmt/Stmt.h"
#include "tree/Expr/Identifier.h"

struct ClassDecl : Stmt {
    id_ptr id;
    id_ptr super_id;
    StmtList fields;

    ClassDecl(const Position & pos, id_ptr id, id_ptr super_id, const StmtList & fields)
        : Stmt(pos, StmtType::Class), id(id), super_id(super_id), fields(fields) {}
    virtual ~ClassDecl() = default;

    void accept(BaseVisitor & visitor) override {
        visitor.visit(this);
    }
};

#endif