#ifndef TREE_TYPE_H
#define TREE_TYPE_H

#include <utility>

#include "tree/Expr/Identifier.h"

namespace jc::tree {
    struct Type;
    struct IdType;
    using type_ptr = std::shared_ptr<Type>;
    using t_list = std::vector<type_ptr>;
    using id_type_ptr = std::shared_ptr<IdType>;

    // TODO: Think if we need Type type :)
    //  Like to statically check what Type node do we use
    struct Type : Node {
        explicit Type(const Position & pos) : Node(pos) {}

        void accept(BaseVisitor & visitor) override = 0;
    };

    struct IdType : Type {
        id_ptr id;

        IdType(const Position & pos, id_ptr id) : Type(pos), id(std::move(id)) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };

    struct ListType : Type {
        type_ptr type;

        ListType(const Position & pos, type_ptr type) : Type(pos), type(std::move(type)) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };

    struct DictType : Type {
        type_ptr key;
        type_ptr val;

        DictType(const Position & pos, type_ptr key, type_ptr val)
            : Type(pos), key(std::move(key)), val(std::move(val)) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };

    struct GenericType : Type {
        id_type_ptr id;
        std::vector<type_ptr> types;

        GenericType(id_type_ptr id, std::vector<type_ptr> types)
            : Type(id->pos), id(std::move(id)), types(std::move(types)) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };

    // UnionType stores not a vector, but left and right types
    // So, `A | B | C` is UnionType(A, UnionType(B, C))
    struct UnionType : Type {
        t_list types;

        explicit UnionType(const t_list & types) : Type(types.at(0)->pos), types(types) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };

    struct FuncType : Type {
        type_ptr return_type;
        t_list params_t;

        FuncType(type_ptr return_type, const t_list & params_t)
            : Type(return_type->pos), return_type(return_type), params_t(params_t) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif // TREE_YPE_H
