#include "compiler/type_class.h"

namespace jc::compiler {
    /////////////
    // Classes //
    /////////////
    type_class_ptr get_cBool() {
        static const type_class_ptr cBool = std::make_shared<TypeClass>();
        cBool->name = "Bool";
        return cBool;
    }

    type_class_ptr get_cInt() {
        static const type_class_ptr cInt = std::make_shared<TypeClass>();
        cInt->name = "Int";
        return cInt;
    }

    type_class_ptr get_cFloat() {
        static const type_class_ptr cFloat = std::make_shared<TypeClass>();
        cFloat->name = "Float";
        return cFloat;
    }

    type_class_ptr get_cString() {
        static const type_class_ptr cString = std::make_shared<TypeClass>();
        cString->name = "String";
        return cString;
    }

    type_class_ptr get_cFunc() {
        static const type_class_ptr cFunc = std::make_shared<TypeClass>();
        cFunc->name = "Func";
        return cFunc;
    }

    type_class_ptr get_cUnion() {
        // TODO: I don't think I need Union class
//        static const type_class_ptr cUnion = std::make_shared<TypeClass>();
//        cUnion->name = "cUnion";
//        return cUnion;
    }

    ///////////
    // Types //
    ///////////
    type_ptr get_any_t() {
        static const type_ptr any_t = std::make_shared<Any>();
        return any_t;
    }

    type_ptr get_null_t() {
        static const type_ptr null_t = std::make_shared<NullType>();
        return null_t;
    }

    type_ptr get_void_t() {
        static const type_ptr void_t = std::make_shared<VoidType>();
        return void_t;
    }

    type_ptr get_bool_t() {
        static const type_ptr bool_t = std::make_shared<BoolType>(get_cBool());
        return bool_t;
    }

    type_ptr get_int_t() {
        static const type_ptr int_t = std::make_shared<IntType>(get_cInt());
        return int_t;
    }

    type_ptr get_float_t() {
        static const type_ptr float_t = std::make_shared<FloatType>(get_cFloat());
        return float_t;
    }

    type_ptr get_string_t() {
        static const type_ptr string_t = std::make_shared<StringType>(get_cString());
        return string_t;
    }

    //////////////////////////
    // Classes Initializers //
    //////////////////////////
    void init_cNull() {
        static bool inited = false;
        if (inited) {
            return;
        }

        // ...

        inited = true;
    }

    void init_cBool() {
        static bool inited = false;
        if (inited) {
            return;
        }

        // ...

        inited = true;
    }

    void init_cInt() {
        static bool inited = false;
        if (inited) {
            return;
        }

        const auto & cInt = get_int_t()->_class;

        cInt->methods.insert({
           "add",
           make_nf_op_t(get_int_t(), {get_int_t()})
        });

        inited = true;
    }

    void init_cFloat() {
        static bool inited = false;
        if (inited) {
            return;
        }

        // ...

        inited = true;
    }

    void init_cString() {
        static bool inited = false;
        if (inited) {
            return;
        }

        const auto & cString = get_string_t()->_class;

        cString->fields.insert({"size", {tree::VarDeclKind::Val, get_int_t()}});

        inited = true;
    }

    /////////////
    // Helpers //
    /////////////
    func_t_ptr make_func_t(const type_ptr & return_type, const t_list & arg_types, bool is_operator, TypeTag callable_type) {
        return std::make_shared<FuncType>(return_type, arg_types, get_cFunc(), is_operator, callable_type);
    }

    func_t_ptr make_nf_t(const type_ptr & return_type, const t_list & arg_types, bool is_operator) {
        return make_func_t(return_type, arg_types, is_operator, TypeTag::NativeFunc);
    }

    func_t_ptr make_nf_op_t(const type_ptr & return_type, const t_list & arg_types) {
        return make_nf_t(return_type, arg_types, true);
    }

    type_ptr make_vararg_t(const type_ptr & vararg_type) {
        return std::make_shared<VarargTagType>(vararg_type);
    }

    func_t_ptr class_has_method(const type_ptr & type, const std::string & method_name, const func_t_ptr & signature) {
        const auto & eq_range = type->_class->methods.equal_range(method_name);

        for (auto it = eq_range.first; it != eq_range.second; it++) {
            if (it->second->compare(signature)) {
                return it->second;
            }
        }

        return nullptr;
    }

    std::string mangle_type(const type_ptr & type, const std::string & name) {
        std::string mangled = "_J";
        mangled += std::to_string(name.size());
        return mangled + mangle_type(type);
    }

    std::string mangle_type(const type_ptr & type) {
        switch (type->tag) {
            case TypeTag::None: {
                throw DevError("Unable to mangle None type");
            }
            case TypeTag::Any: {
                return "Any";
            }
            case TypeTag::Null: {
                return "Null";
            }
            case TypeTag::Void: {
                return "Void";
            }
            case TypeTag::Bool: {
                return "Bool";
            }
            case TypeTag::Int: {
                return "Int";
            }
            case TypeTag::Float: {
                return "Float";
            }
            case TypeTag::String: {
                return "String";
            }
            case TypeTag::Func:
            case TypeTag::NativeFunc: {
                return mangle_type(std::static_pointer_cast<FuncType>(type));
            }
//            case TypeTag::Class: {
//                // TODO: Class type
////                return mangle_type(std::static_pointer_cast<ClassType>(type));
//            }
            case TypeTag::VarargTag: {
                return mangle_type(std::static_pointer_cast<VarargTagType>(type)->vararg_type);
            }
            case TypeTag::Union: {
                // TODO
            }
            default: {
                throw DevError("Unsupported type for mangling");
            }
        }
    }

    std::string mangle_type(const func_t_ptr & func_t) {
        std::string mangled_func_t = mangle_type(func_t->return_type);
        mangled_func_t += "(";
        for (size_t i = 0; i < func_t->arg_types.size(); i++) {
            mangled_func_t += mangle_type(func_t->arg_types.at(i));
            if (i < func_t->arg_types.size() - 1) {
                mangled_func_t += ",";
            }
        }
        mangled_func_t += ")";
        return mangled_func_t;
    }
}
