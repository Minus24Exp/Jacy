#ifndef TYPE_H
#define TYPE_H

#include "tree/nodes.h"

#include <string>
#include <memory>
#include <map>

// TODO: ! Move all structs to top and constants to bottom

struct Type;
struct Class;
using class_ptr = std::shared_ptr<Class>;
using type_ptr = std::shared_ptr<Type>;

enum class TypeTag {
    None,
    Any,
    Null,
    Void,
    Bool,
    Int,
    Float,
    String,
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

struct NullType : Type {
    NullType(const class_ptr & cNull) : Type(TypeTag::Null, cNull) {}

    bool compare(const type_ptr & other) override {
        return other->tag == TypeTag::Null;
    }
};

struct NullableType : Type {
    explicit NullableType(const type_ptr & type) : Type(type->tag, type->_class), type(type) {}

    type_ptr type;

    bool compare(const type_ptr & other) override {
        return other->tag == TypeTag::Null || other->compare(type);
    }
};

struct VoidType : Type {
    VoidType() : Type(TypeTag::Void, nullptr) {}

    bool compare(const type_ptr & other) override {
        return other->tag == TypeTag::Void;
    }
};

struct BoolType : Type {
    BoolType(const class_ptr & cBool) : Type(TypeTag::Bool, cBool) {}

    bool compare(const type_ptr & other) override {
        return other->tag == TypeTag::Bool;
    }
};

struct IntType : Type {
    IntType(const class_ptr & cInt) : Type(TypeTag::Int, cInt) {}

    bool compare(const type_ptr & other) override {
        return other->tag == TypeTag::Int;
    }
};

struct FloatType : Type {
    FloatType(const class_ptr & cFloat) : Type(TypeTag::Float, cFloat) {}

    bool compare(const type_ptr & other) override {
        return other->tag == TypeTag::Float;
    }
};

struct StringType : Type {
    StringType(const class_ptr & cString) : Type(TypeTag::String, cString) {}

    bool compare(const type_ptr & other) override {
        return other->tag == TypeTag::String;
    }
};


struct FuncType;
using func_t_ptr = std::shared_ptr<FuncType>;
using t_list = std::vector<type_ptr>;

struct FuncType : Type {
    FuncType(TypeTag callable_type, const type_ptr & return_type, const t_list & arg_types, const class_ptr & cFunc)
        : Type(callable_type, cFunc), return_type(return_type), arg_types(arg_types) {}

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
    explicit UnionType(t_list && types, const class_ptr & cUnion) : Type(TypeTag::Union, cUnion), types(std::move(types)) {}

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

#endif // CLASS_H
