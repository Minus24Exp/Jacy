#ifndef TREE_TYPE_H
#define TREE_TYPE_H

#include "tree/Expr/Identifier.h"

namespace jc::tree {
    struct Type;
    struct IdType;
    using type_ptr = std::shared_ptr<Type>;
    using id_type_ptr = std::shared_ptr<IdType>;

    // TODO: Think if we need Type type :)
    //  Like to statically check what Type node do we use
    struct Type : Node {
        Type(const Position & pos) : Node(pos) {}

        virtual void accept(BaseVisitor & visitor) = 0;
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

        GenericType(id_type_ptr id, const std::vector<type_ptr> & types) : Type(id->pos), id(id), types(types) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };

    // UnionType stores not a vector, but left and right types
    // So, `A | B | C` is UnionType(A, UnionType(B, C))
    struct UnionType : Type {
        type_ptr left;
        type_ptr right;

        UnionType(type_ptr left, type_ptr right) : Type(left->pos), left(std::move(left)), right(std::move(right)) {}

        void accept(BaseVisitor & visitor) override {
            visitor.visit(this);
        }
    };
}

#endif // TREE_YPE_H
