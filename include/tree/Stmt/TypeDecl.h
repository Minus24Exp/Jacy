#ifndef TYPEDECL_H
#define TYPEDECL_H

#include "tree/Stmt/Stmt.h"

namespace jc::tree {
    struct TypeDecl : Stmt {
        id_ptr id;
        expr_ptr type_expr;

        TypeDecl(const Position & pos, id_ptr id, expr_ptr type_expr)
                : Stmt(pos, StmtType::Type), id(id), type_expr(type_expr) {}
        virtual ~TypeDecl() = default;

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif