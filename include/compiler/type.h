#ifndef TYPE_H
#define TYPE_H

#include "tree/nodes.h"
#include "Exception.h"

#include <string>
#include <memory>
#include <map>

namespace jc::compiler {
    // TODO: ! Move all structs to top and constants to bottom

    struct Type;
    struct FuncType;
    using type_ptr = std::shared_ptr<Type>;
    using t_list = std::vector<type_ptr>;
    using func_t_ptr = std::shared_ptr<FuncType>;

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
        UserDefined,
        VarargTag,
        Union,
    };

    struct Field {
        tree::VarDeclKind kind;
        type_ptr type;
//        std::string name;
    };

    struct Type {
        explicit Type(TypeTag tag) : tag(tag) {}

        TypeTag tag{TypeTag::None};
        std::string name;
        std::map<std::string, Field> fields;
        std::multimap<std::string, func_t_ptr> methods;

        virtual bool compare(const type_ptr & other) = 0;

        // TODO: Type-checking prod to_string
        // Now for debug only
        virtual std::string to_string() = 0;
    };

    struct Any : Type {
        Any() : Type(TypeTag::Any) {}

        bool compare(const type_ptr & other) override {
            return true;
        }

        std::string to_string() override {
            return "any";
        }
    };

    struct NullType : Type {
        NullType() : Type(TypeTag::Null) {}

        bool compare(const type_ptr & other) override {
            return other->tag == TypeTag::Null;
        }

        std::string to_string() override {
            return "null";
        }
    };

    // TODO: Remove nullable type (it's just union type with null)
    struct NullableType : Type {
        explicit NullableType(const type_ptr & type) : Type(type->tag), type(type) {}

        type_ptr type;

        bool compare(const type_ptr & other) override {
            return other->tag == TypeTag::Null || other->compare(type);
        }

        std::string to_string() override {
            return "AAA";
        }
    };

    struct VoidType : Type {
        VoidType() : Type(TypeTag::Void) {}

        bool compare(const type_ptr & other) override {
            return other->tag == TypeTag::Void;
        }

        std::string to_string() override {
            return "void";
        }
    };

    struct BoolType : Type {
        explicit BoolType() : Type(TypeTag::Bool) {}

        bool compare(const type_ptr & other) override {
            return other->tag == TypeTag::Bool;
        }

        std::string to_string() override {
            return "bool";
        }
    };

    struct IntType : Type {
        explicit IntType() : Type(TypeTag::Int) {}

        bool compare(const type_ptr & other) override {
            return other->tag == TypeTag::Int;
        }

        std::string to_string() override {
            return "int";
        }
    };

    struct FloatType : Type {
        explicit FloatType() : Type(TypeTag::Float) {}

        bool compare(const type_ptr & other) override {
            return other->tag == TypeTag::Float;
        }

        std::string to_string() override {
            return "float";
        }
    };

    struct StringType : Type {
        explicit StringType() : Type(TypeTag::String) {}

        bool compare(const type_ptr & other) override {
            return other->tag == TypeTag::String;
        }

        std::string to_string() override {
            return "string";
        }
    };

    struct FuncParamType : Type {
        FuncParamType(const type_ptr & type, bool has_default_val) : Type(type->tag), type(type), has_default_val(has_default_val) {}

        type_ptr type;
        bool has_default_val;

        bool compare(const type_ptr & other) override {
            return type->compare(other);
        }

        std::string to_string() override {
            return type->to_string() + (has_default_val ? ":default" : "");
        }
    };

    struct FuncType : Type {
        FuncType(
            const type_ptr & return_type,
            const t_list & arg_types,
            bool is_operator = false,
            TypeTag callable_type = TypeTag::Func
        ) : Type(callable_type),
            return_type(return_type),
            arg_types(arg_types),
            is_operator(is_operator) {}

        type_ptr return_type;
        // TODO: Default values
        t_list arg_types;
        bool is_operator{false};

        bool compare(const type_ptr & other) override {
            compare(other, false);
        }

        // Note: Use this function mostly always instead of overridden compare
        bool compare(const type_ptr & other, bool is_op_optional) {
            if (other->tag != TypeTag::Func && other->tag != TypeTag::NativeFunc) {
                return false;
            }
            std::shared_ptr<FuncType> func_type = std::static_pointer_cast<FuncType>(other);

            // Compare for operator attribute only in FuncType-FuncType comparison
            // Avoid this comparison if operator flag is optional
            if (!is_op_optional && func_type->is_operator != is_operator) {
                return false;
            }

            return compare(func_type->return_type, func_type->arg_types);
        }

        // FuncType-FuncType comparison (may be used for similar function searching)
        bool compare(const type_ptr & _return_type, const std::vector<type_ptr> & other_arg_types) {
            // Return type
            if (!return_type->compare(_return_type)) {
                return false;
            }
            return compare(other_arg_types);
        }

        // FuncCall comparison
        bool compare(const std::vector<type_ptr> & other_arg_types) {
            if (arg_types.size() > other_arg_types.size() || arg_types.empty() && !other_arg_types.empty()) {
                return false;
            }

            size_t index = 0;
            for (size_t arg_t_i = 0; arg_t_i < arg_types.size(); arg_t_i++) {
                const auto & cur_t = arg_types.at(arg_t_i);
                type_ptr vararg_t = cur_t->tag == TypeTag::VarargTag ? cur_t : nullptr;
                if (vararg_t) {
                    const auto & cmp_t = other_arg_types.at(index);
                    if (!vararg_t->compare(cmp_t)) {
                        return false;
                    }
                    while (index < other_arg_types.size() && vararg_t->compare(cmp_t)) {
                        index++;
                    }
                    // If next type after vararg is the same as vararg_t, go to previous type
                    if (arg_t_i + 1 < arg_types.size()
                        && vararg_t->compare(arg_types.at(arg_t_i + 1))) {
                        index--;
                    }
                } else if (!cur_t->compare(other_arg_types.at(index))) {
                    return false;
                } else {
                    // Go to next if not vararg, but single type was right
                    index++;
                }
                // Go to next type for comparison
                arg_t_i++;
            }

            return true;
        }

        std::string to_string() override {
            std::string arg_types_str;
            for (int i = 0; i < arg_types.size(); i++) {
                arg_types_str += arg_types.at(i)->to_string();
                if (i < arg_types.size() - 1) {
                    arg_types_str += ", ";
                }
            }
            return "func_type:" + return_type->to_string() + "(" + arg_types_str + ")";
        }
    };

    struct VarargTagType : Type {
        explicit VarargTagType(const type_ptr & vararg_type) : Type(TypeTag::VarargTag), vararg_type(vararg_type) {}

        type_ptr vararg_type;

        bool compare(const type_ptr & other) override {
            return vararg_type->compare(other);
        }

        std::string to_string() override {
            return "vararg:" + vararg_type->to_string();
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

        std::string to_string() override {
            std::string united = "union:";
            for (size_t i = 0; i < types.size(); i++) {
                united += types.at(i)->to_string();
                if (i < types.size() - 1) {
                    united += " | ";
                }
            }
            return united;
        }
    };

    // Type getters //
    type_ptr get_any_t();
    type_ptr get_void_t();
    type_ptr get_null_t();
    type_ptr get_bool_t();
    type_ptr get_int_t();
    type_ptr get_float_t();
    type_ptr get_string_t();
}

#endif // TYPE_CLASS_H
