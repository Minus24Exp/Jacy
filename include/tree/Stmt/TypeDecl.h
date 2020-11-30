#ifndef TYPEDECL_H
#define TYPEDECL_H

#include <utility>

#include "tree/Stmt/Stmt.h"
#include "tree/Type.h"

namespace jc::tree {
    struct TypeDecl : Stmt {
        id_ptr id;
        type_ptr type;

        TypeDecl(const Position & pos, id_ptr id, type_ptr type)
            : Stmt(pos, StmtType::Type), id(std::move(id)), type(std::move(type)) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif