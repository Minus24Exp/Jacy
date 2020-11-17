#ifndef TYPES_H
#define TYPES_H

#include "tree/nodes.h"

#include <string>
#include <memory>
#include <map>

// TODO: ! Move all structs to top and constants to bottom

struct Type;
struct Class;
using class_ptr = std::shared_ptr<Class>;
using type_ptr = std::shared_ptr<Type>;

extern class_ptr cAny;
extern class_ptr cNull;
extern class_ptr

enum class TypeTag {
    None,
    Any,
    Null,
    Void,
    Primitive,
    Func,
    NativeFunc,
    Class,
    Union,
};

struct Type {
    explicit Type(TypeTag tag, const class_ptr & _class) : tag(tag), _class(_class) {}

    TypeTag tag{TypeTag::None};
    class_ptr _class;

    virtual bool compare(const type_ptr & other) = 0;
};

struct Any : Type {
    Any() : Type(TypeTag::Any, nullptr) {}

    bool compare(const type_ptr & other) override {
        return true;
    }
};
const type_ptr any_t = std::make_shared<Any>();

struct NullType : Type {
    NullType() : Type(TypeTag::Null, cNull) {}

    bool compare(const type_ptr & other) override {
        return other->tag == TypeTag::Null;
    }
};
const type_ptr null_t = std::make_shared<NullType>();

struct VoidType : Type {
    VoidType() : Type(TypeTag::Void) {}

    bool compare(const type_ptr & other) override {
        return other->tag == TypeTag::Void;
    }
};
const type_ptr void_t = std::make_shared<VoidType>();

struct NullableType : Type {
    explicit NullableType(const type_ptr & type) : Type(type->tag), type(type) {}

    type_ptr type;

    bool compare(const type_ptr & other) override {
        return other->tag == TypeTag::Null || other->compare(type);
    }
};

enum class Primitive {
    Bool,
    Int,
    Float,
    String,
};

struct PrimitiveType : Type {
    explicit PrimitiveType(Primitive type) : Type(TypeTag::Primitive), type(type) {}

    Primitive type;

    bool compare(const type_ptr & other) override {
        return other->tag == TypeTag::Primitive && std::static_pointer_cast<PrimitiveType>(other)->type == type;
    }
};
const type_ptr bool_t = std::make_shared<PrimitiveType>(Primitive::Bool);
const type_ptr int_t = std::make_shared<PrimitiveType>(Primitive::Int);
const type_ptr float_t = std::make_shared<PrimitiveType>(Primitive::Float);
const type_ptr string_t = std::make_shared<PrimitiveType>(Primitive::String);

struct FuncType;
using func_t_ptr = std::shared_ptr<FuncType>;
using t_list = std::vector<type_ptr>;

struct FuncType : Type {
    FuncType(TypeTag callable_type, const type_ptr & return_type, const t_list & arg_types)
        : Type(callable_type), return_type(return_type), arg_types(arg_types) {}

    type_ptr return_type;
    t_list arg_types;

    bool compare(const type_ptr & other) override {
        if (other->tag != TypeTag::Func &&
            other->tag != TypeTag::NativeFunc &&
            other->tag != TypeTag::Class) {
            return false;
        }
        std::shared_ptr<FuncType> func_type = std::static_pointer_cast<FuncType>(other);
        return compare(func_type->return_type, func_type->arg_types);
    }

    bool compare(const type_ptr & _return_type, const std::vector<type_ptr> & _arg_types) {
        // TODO: Rewrite when varargs will be implemented
        if (arg_types.size() != _arg_types.size()) {
            return false;
        }
        for (uint64_t i = 0; i < arg_types.size(); i++) {
            if (!arg_types.at(i)->compare(_arg_types.at(i))) {
                return false;
            }
        }
        return _return_type == return_type;
    }
};

struct UnionType : Type {
    explicit UnionType(t_list && types) : Type(TypeTag::Union), types(std::move(types)) {}

    t_list types;

    bool compare(const type_ptr & other) override {
        if (other->tag == TypeTag::Union) {
            const auto & other_union = std::static_pointer_cast<UnionType>(other);
            // TODO: Type inheritance check
            for (int i = 0; i < types.size(); i++) {
                for (int j = i; j < other_union->types.size(); j++) {
                    if (types[i]->compare(other_union->types[j])) {
                        return true;
                    }
                }
            }
        } else {
            for (const auto & type : types) {
                if (type->compare(other)) {
                    return true;
                }
            }
        }

        return false;
    }
};

#endif // TYPES_H
