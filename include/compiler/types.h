#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <memory>
#include "tree/nodes.h"

struct Type;
using type_ptr = std::shared_ptr<Type>;

enum class TypeTag {
    None,
    Primitive,
};

struct Type {
    TypeTag tag{TypeTag::None};
    virtual bool compare(const type_ptr & other) = 0;
};

struct Any : Type {
    bool compare(const type_ptr & other) override {
        return true;
    }
};
const type_ptr any_t = std::make_shared<Any>();

enum class Primitive {
    Bool,
    Int,
    Float,
    String,
};

struct PrimitiveType : Type {
    Primitive type;

    bool compare(const type_ptr & other) override {
        return other->tag == TypeTag::Primitive && std::static_pointer_cast<PrimitiveType>(other)->type == type;
    }
};

#endif // TYPES_H
