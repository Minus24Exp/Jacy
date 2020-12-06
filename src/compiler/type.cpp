#include "compiler/type.h"

namespace jc::compiler {
    // Type //
    Type::Type(TypeTag tag) : tag(tag) {}

    std::string Type::mangle_name(const std::string & t_name) {
        std::string prefix = "_J" + t_name;
        return prefix + mangle();
    }

    func_t_ptr Type::has_method(const std::string &method_name, const func_t_ptr &signature, bool is_op_optional) {
        // TODO: Improve for most inherited types,
        //  for a(int) call must be used a(int) if exists, not a(any)

        const auto & eq_range = methods.equal_range(method_name);

        for (auto it = eq_range.first; it != eq_range.second; it++) {
            if (it->second->compare(signature, is_op_optional)) {
                return it->second;
            }
        }

        return nullptr;
    }

    // Nothing //
    Nothing::Nothing() : Type(TypeTag::Bottom) {}

    type_ptr Nothing::get() {
        static type_ptr nothing_t = std::make_shared<Nothing>();
        return nothing_t;
    }

    bool Nothing::equals(const type_ptr & other) {
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

    type_ptr UnitType::get() {
        static type_ptr unit_t = std::make_shared<UnitType>();
        return unit_t;
    }

    bool UnitType::equals(const type_ptr & other) {
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

    type_ptr NullType::get() {
        static type_ptr null_t = std::make_shared<NullType>();
        return null_t;
    }

    bool NullType::equals(const type_ptr & other) {
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

    type_ptr BoolType::get() {
        static type_ptr bool_t = std::make_shared<BoolType>();
        return bool_t;
    }

    bool BoolType::equals(const type_ptr & other) {
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

    type_ptr IntType::get() {
        static type_ptr int_t = std::make_shared<IntType>();
        return int_t;
    }

    bool IntType::equals(const type_ptr & other) {
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

    type_ptr FloatType::get() {
        static type_ptr float_t = std::make_shared<FloatType>();
        return float_t;
    }

    bool FloatType::equals(const type_ptr & other) {
        return other->tag == TypeTag::Float;
    }

    std::string FloatType::to_string() {
        return "Float";
    }

    std::string FloatType::mangle() {
        return "Float";
    }

    // String //
    StringType::StringType() : Type(TypeTag::String) {}

    type_ptr StringType::get() {
        static type_ptr string_t = std::make_shared<StringType>();
        return string_t;
    }

    bool StringType::equals(const type_ptr & other) {
        return other->tag == TypeTag::String;
    }

    std::string StringType::to_string() {
        return "String";
    }

    std::string StringType::mangle() {
        return "String";
    }

    // FuncParam //
    FuncParamType::FuncParamType(const type_ptr & type, bool has_default_val)
        : Type(type->tag), type(type), has_default_val(has_default_val) {}

    func_param_t_ptr FuncParamType::get(const type_ptr & type, bool has_default_val) {
        return std::make_shared<FuncParamType>(type, has_default_val);
    }

    bool FuncParamType::equals(const type_ptr & other) {
        return type->equals(other);
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

    func_t_ptr FuncType::get(const type_ptr & return_type, const func_param_t_list & arg_types, bool is_operator, TypeTag callable_type) {
        return std::make_shared<FuncType>(return_type, arg_types, is_operator, callable_type);
    }

    func_t_ptr FuncType::get_nf_t(const type_ptr & return_type, const func_param_t_list & arg_types, bool is_operator) {
        return get(return_type, arg_types, is_operator, TypeTag::NativeFunc);
    }

    func_t_ptr FuncType::get_nf_op_t(const type_ptr & return_type, const func_param_t_list & arg_types) {
        return get_nf_t(return_type, arg_types, true);
    }

    bool FuncType::equals(const type_ptr & other) {
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
        if (!return_type->equals(_return_type)) {
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
                if (!vararg_t->equals(cmp_t)) {
                    return false;
                }
                while (index < other_arg_types.size() && vararg_t->equals(cmp_t)) {
                    index++;
                }
                // If next type after vararg is the same as vararg_t, go to previous type
                if (arg_t_i + 1 < arg_types.size()
                    && vararg_t->equals(arg_types.at(arg_t_i + 1))) {
                    index--;
                }
            } else if (!cur_t->equals(other_arg_types.at(index))) {
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

    // Any //
    Any::Any() : Type(TypeTag::Any) {}

    type_ptr Any::get() {
        static type_ptr any_t = std::make_shared<Any>();
        return any_t;
    }

    bool Any::equals(const type_ptr & other) {
        return true;
    }

    std::string Any::to_string() {
        return "Any";
    }

    std::string Any::mangle() {
        return "Any";
    }

    // Vararg //
    VarargType::VarargType(const type_ptr & vararg_type) : Type(TypeTag::VarargTag), vararg_type(vararg_type) {}

    type_ptr VarargType::get(const type_ptr & vararg_type) {
        return std::make_shared<VarargType>(vararg_type);
    }

    bool VarargType::equals(const type_ptr & other) {
        return vararg_type->equals(other);
    }

    std::string VarargType::to_string() {
        return "vararg:" + vararg_type->to_string();
    }

    std::string VarargType::mangle() {
        return "[vararg:" + vararg_type->mangle() + "]";
    }

    // ListType //
    ListType::ListType(const type_ptr & type) : Type(TypeTag::List), type(type) {}

    type_ptr ListType::get(const type_ptr & type) {
        return std::make_shared<ListType>(type);
    }

    bool ListType::equals(const type_ptr & other) {
        if (other->tag != TypeTag::List) {
            return false;
        }

        const auto & other_list_t = std::static_pointer_cast<ListType>(other);
        return type->equals(other_list_t->type);
    }

    std::string ListType::to_string() {
        return "[" + type->to_string() + "]";
    }

    std::string ListType::mangle() {
        return "[" + type->mangle() + "]";
    }

    // DictType //
    DictType::DictType(const type_ptr & key, const type_ptr & val)
            : Type(TypeTag::Dict), key(key), val(val) {}

    type_ptr DictType::get(const type_ptr & key, const type_ptr & value) {
        return std::make_shared<DictType>(key, value);
    }

    bool DictType::equals(const type_ptr & other) {
        if (other->tag != TypeTag::Dict) {
            return false;
        }

        const auto & other_dict_t = std::static_pointer_cast<DictType>(other);
        return key->equals(other_dict_t->key) && val->equals(other_dict_t->val);
    }

    std::string DictType::to_string() {
        return "{" + key->to_string() + ": " + val->to_string() + "}";
    }

    std::string DictType::mangle() {
        return "{" + key->mangle() + ":" + val->mangle() + "}";
    }

    // GenericType //
    GenericType::GenericType(const type_ptr & generic, const t_list & types)
            : Type(TypeTag::Generic), generic(generic), types(types) {}

    type_ptr GenericType::get(const type_ptr & generic, const t_list & types) {
        return std::make_shared<GenericType>(generic, types);
    }

    bool GenericType::equals(const type_ptr & other) {
        if (other->tag != TypeTag::Generic) {
            return false;
        }

        const auto & other_generic_t = std::static_pointer_cast<GenericType>(other);
        if (!generic->equals(other_generic_t)) {
            return false;
        }

        if (types.size() != other_generic_t->types.size()) {
            return false;
        }

        for (size_t i = 0; i < types.size(); i++) {
            if (!types.at(i)->equals(other_generic_t->types.at(i))) {
                return false;
            }
        }

        return true;
    }

    std::string GenericType::to_string() {
        std::string str = generic->to_string() + "<";
        for (size_t i = 0; i < types.size(); i++) {
            str += types.at(i)->to_string();
            if (i < types.size() - 1) {
                str += ", ";
            }
        }
        return str + ">";
    }

    std::string GenericType::mangle() {
        std::string str = generic->mangle() + "<";
        for (size_t i = 0; i < types.size(); i++) {
            str += types.at(i)->mangle();
            if (i < types.size() - 1) {
                str += ",";
            }
        }
        return str + ">";
    }

    // Union //
    UnionType::UnionType(const t_list & types) : Type(TypeTag::Union), types(types) {}

    type_ptr UnionType::get(const t_list & types) {
        return std::make_shared<UnionType>(types);
    }

    type_ptr UnionType::get_nullable_t(const type_ptr & type) {
        return std::make_shared<UnionType>(t_list{type, NullType::get()});
    }

    bool UnionType::equals(const type_ptr & other) {
        if (other->tag != TypeTag::Union) {
            return false;
        }

        const auto & other_union_t = std::static_pointer_cast<UnionType>(other);
        for (size_t i = 0; i < types.size(); i++) {
            if (!types.at(i)->equals(other_union_t->types.at(i))) {
                return false;
            }
        }

        return true;

//        if (other->tag == TypeTag::Union) {
//            const auto & other_union = std::static_pointer_cast<UnionType>(other);
//            // TODO: Type inheritance check
//            for (int i = 0; i < types.size(); i++) {
//                for (int j = i; j < other_union->types.size(); j++) {
//                    if (types[i]->compare(other_union->types[j])) {
//                        return true;
//                    }
//                }
//            }
//        } else {
//            for (const auto & type : types) {
//                if (type->compare(other)) {
//                    return true;
//                }
//            }
//        }
//
//        return false;
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
}
