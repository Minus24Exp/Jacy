#ifndef FUNCDECL_H
#define FUNCDECL_H

#include <utility>

#include "tree/Expr/Identifier.h"
#include "tree/Stmt/Block.h"
#include "tree/TypeAnno.h"

namespace jc::tree {
    struct FuncParam {
        id_ptr id;
        expr_ptr default_val;
        bool vararg;

        TypeAnno type;
    };

    using FuncParams = std::vector<FuncParam>;

    struct FuncDecl : Stmt {
        id_ptr id;
        FuncParams params;
        block_ptr body;
        TypeAnno return_type;

        FuncDecl(const Position & pos, id_ptr id, FuncParams params, block_ptr body, const TypeAnno & return_type)
            : Stmt(pos, StmtType::FuncDecl),
              id(std::move(id)), params(std::move(params)),
              body(std::move(body)),
              return_type(return_type) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif