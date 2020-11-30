#ifndef TREE_TYPE_H
#define TREE_TYPE_H

#include "tree/Expr/Identifier.h"

namespace jc::tree {
    struct Type;
    struct IdType;
    using type_ptr = std::shared_ptr<Type>;
    using id_type_ptr = std::shared_ptr<IdType>;

    struct Type {};

    struct IdType : Type {
        id_ptr id;

        IdType(id_ptr id) : id(std::move(id)) {}
    };

    struct ListType : Type {
        type_ptr type;

        ListType(type_ptr type) : type(std::move(type)) {}
    };

    struct DictType : Type {
        type_ptr key;
        type_ptr val;

        DictType(type_ptr key, type_ptr val) : key(std::move(key)), val(std::move(val)) {}
    };

    struct GenericType : Type {
        id_type_ptr id;
        std::vector<type_ptr> types;

        GenericType(id_type_ptr id, const std::vector<type_ptr> & types) : id(id), types(types) {}
    };

    // UnionType stores not a vector, but left and right types
    // So, `A | B | C` is UnionType(A, UnionType(B, C))
    struct UnionType : Type {
        type_ptr left;
        type_ptr right;

        UnionType(type_ptr left, type_ptr right) : left(std::move(left)), right(std::move(right)) {}
    };
}

#endif // TREE_YPE_H
