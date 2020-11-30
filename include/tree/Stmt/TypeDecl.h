#ifndef TYPEDECL_H
#define TYPEDECL_H

#include <utility>

#include "tree/Stmt/Stmt.h"
#include "tree/TypeAnno.h"

namespace jc::tree {
    struct TypeDecl : Stmt {
        id_ptr id;
        TypeAnno type;

        TypeDecl(const Position & pos, id_ptr id, const TypeAnno & type)
                : Stmt(pos, StmtType::Type), id(std::move(id)), type(type) {}
        ~TypeDecl() override = default;

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif