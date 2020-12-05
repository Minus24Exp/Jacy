#include "compiler/type.h"

namespace jc::compiler {
    // Type //
    Type::Type(TypeTag tag) : tag(tag) {}

    std::string Type::mangle(const std::string & t_name) {
        std::string prefix = "_J" + t_name;
        return prefix + mangle();
    }

    // Nothing //
    Nothing::Nothing() : Type(TypeTag::Bottom) {}

    bool Nothing::compare(const type_ptr & other) {
        return true; // Stub
    }

    std::string Nothing::to_string() {
        return "Nothing";
    }

    std::string Nothing::mangle() {
        return "Nothing";
    }

    // Unit //
    UnitType::UnitType() : Type(TypeTag::Unit) {}

    bool UnitType::compare(const type_ptr & other) {
        return other->tag == TypeTag::Unit;
    }

    std::string UnitType::to_string() {
        return "Unit";
    }

    std::string UnitType::mangle() {
        return "Unit";
    }

    // Null //
    NullType::NullType() : Type(TypeTag::Null) {}

    bool NullType::compare(const type_ptr & other) {
        return other->tag == TypeTag::Null;
    }

    std::string NullType::to_string() {
        return "Null";
    }

    std::string NullType::mangle() {
        return "Null";
    }

    // BoolType //
    BoolType::BoolType() : Type(TypeTag::Bool) {}

    bool BoolType::compare(const type_ptr & other) {
        return other->tag == TypeTag::Bool;
    }

    std::string BoolType::to_string() {
        return "Bool";
    }

    std::string BoolType::mangle() {
        return "Bool";
    }

    // Int //
    IntType::IntType() : Type(TypeTag::Int) {}

    bool IntType::compare(const type_ptr & other) {
        return other->tag == TypeTag::Int;
    }

    std::string IntType::to_string() {
        return "Int";
    }

    std::string IntType::mangle() {
        return "Int";
    }

    // Float //
    FloatType::FloatType() : Type(TypeTag::Float) {}

    bool FloatType::compare(const type_ptr & other) {
        return other->tag == TypeTag::Float;
    }

    std::string FloatType::to_string() {
        return "Float";
    }

    std::string FloatType::mangle() {
        return "Float";
    }

    // String //
    bool StringType::compare(const type_ptr & other) {
        return other->tag == TypeTag::String;
    }

    StringType::StringType() : Type(TypeTag::String) {}

    std::string StringType::to_string() {
        return "String";
    }

    std::string StringType::mangle() {
        return "String";
    }

    // FuncParam //
    FuncParamType::FuncParamType(const type_ptr & type, bool has_default_val)
        : Type(type->tag), type(type), has_default_val(has_default_val) {}

    bool FuncParamType::compare(const type_ptr & other) {
        return type->compare(other);
    }

    std::string FuncParamType::to_string() {
        return type->to_string() + (has_default_val ? ":default" : "");
    }

    std::string FuncParamType::mangle() {
        return type->mangle();
    }

    // Func //
    FuncType::FuncType(const type_ptr & return_type, const func_param_t_list & arg_types, bool is_operator, TypeTag callable_type)
            : Type(callable_type),
              return_type(return_type),
              arg_types(arg_types),
              is_operator(is_operator) {}

    bool FuncType::compare(const type_ptr & other) {
        compare(other, false);
    }

    bool FuncType::compare(const type_ptr & other, bool is_op_optional) {
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

    bool FuncType::compare(const type_ptr & _return_type, const func_param_t_list & other_arg_types) {
        // Return type
        if (!return_type->compare(_return_type)) {
            return false;
        }
        return compare(other_arg_types);
    }

    bool FuncType::compare(const func_param_t_list & other_arg_types) {
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

    std::string FuncType::to_string() {
        std::string arg_types_str;
        for (int i = 0; i < arg_types.size(); i++) {
            arg_types_str += arg_types.at(i)->to_string();
            if (i < arg_types.size() - 1) {
                arg_types_str += ", ";
            }
        }
        return "func_type:" + return_type->to_string() + "(" + arg_types_str + ")";
    }

    std::string FuncType::mangle() {
        std::string mangled_func_t;
        if (is_operator) {
            mangled_func_t += "[op]";
        }
        mangled_func_t += return_type->mangle() + "(";
        for (size_t i = 0; i < arg_types.size(); i++) {
            mangled_func_t += arg_types.at(i)->mangle();
            if (i < arg_types.size() - 1) {
                mangled_func_t += ",";
            }
        }
        mangled_func_t += ")";
        return mangled_func_t;
    }

    func_t_ptr FuncType::get_func_t(const type_ptr & return_type, const func_param_t_list & arg_types, bool is_operator, TypeTag callable_type) {
        return std::make_shared<FuncType>(return_type, arg_types, is_operator, callable_type);
    }

    func_t_ptr FuncType::get_nf_t(const type_ptr & return_type, const func_param_t_list & arg_types, bool is_operator) {
        return get_func_t(return_type, arg_types, is_operator, TypeTag::NativeFunc);
    }

    func_t_ptr FuncType::get_nf_op_t(const type_ptr & return_type, const func_param_t_list & arg_types) {
        return get_nf_t(return_type, arg_types, true);
    }

    // Any //
    Any::Any() : Type(TypeTag::Any) {}

    bool Any::compare(const type_ptr & other) {
        return true;
    }

    std::string Any::to_string() {
        return "Any";
    }

    std::string Any::mangle() {
        return "Any";
    }

    // Vararg //
    VarargTagType::VarargTagType(const type_ptr & vararg_type) : Type(TypeTag::VarargTag), vararg_type(vararg_type) {}

    bool VarargTagType::compare(const type_ptr & other) {
        return vararg_type->compare(other);
    }

    std::string VarargTagType::to_string() {
        return "vararg:" + vararg_type->to_string();
    }

    std::string VarargTagType::mangle() {
        return "[vararg:" + vararg_type->mangle() + "]";
    }

    type_ptr VarargTagType::get_vararg_t(const type_ptr & vararg_type) {
        return std::make_shared<VarargTagType>(vararg_type);
    }

    // Union //
    UnionType::UnionType(t_list && types) : Type(TypeTag::Union), types(std::move(types)) {}

    bool UnionType::compare(const type_ptr & other) {
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

    std::string UnionType::to_string() {
        std::string united = "union:";
        for (size_t i = 0; i < types.size(); i++) {
            united += types.at(i)->to_string();
            if (i < types.size() - 1) {
                united += " | ";
            }
        }
        return united;
    }

    std::string UnionType::mangle() {
        std::string united = "[union:";
        for (size_t i = 0; i < types.size(); i++) {
            united += types.at(i)->to_string();
            if (i < types.size() - 1) {
                united += "|";
            }
        }
        return united + "]";
    }

    type_ptr UnionType::get_nullable_t(const type_ptr & type) {
        return std::make_shared<UnionType>(t_list{type, get_null_t()});
    }
}
