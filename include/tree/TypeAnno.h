#ifndef TYPEANNO_H
#define TYPEANNO_H

#include "tree/Expr/Identifier.h"

namespace jc::tree {
    struct TypeAnno {};

    struct IdType : TypeAnno {
        id_ptr id;
    };

    struct ArrayType : TypeAnno {
        TypeAnno type;
    };

    struct UnionType : TypeAnno {
        std::vector<expr_ptr> types;
    };
}

#endif // TYPEANNO_H
