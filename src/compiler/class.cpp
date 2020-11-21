#include "compiler/class.h"


/////////////
// Classes //
/////////////
class_ptr get_cNull() {
    static const class_ptr cNull = std::make_shared<Class>();
    return cNull;
}

class_ptr get_cBool() {
    static const class_ptr cBool = std::make_shared<Class>();
    return cBool;
}

class_ptr get_cInt() {
    static const class_ptr cInt = std::make_shared<Class>();
    return cInt;
}

class_ptr get_cFloat() {
    static const class_ptr cFloat = std::make_shared<Class>();
    return cFloat;
}

class_ptr get_cString() {
    static const class_ptr cString = std::make_shared<Class>();
    return cString;
}

class_ptr get_cFunc() {
    static const class_ptr cFunc = std::make_shared<Class>();
    return cFunc;
}

class_ptr get_cUnion() {
    static const class_ptr cUnion = std::make_shared<Class>();
    return cUnion;
}

///////////
// Types //
///////////
type_ptr get_any_t() {
    static const type_ptr any_t = std::make_shared<Any>();
    return any_t;
}

type_ptr get_null_t() {
    static const type_ptr null_t = std::make_shared<NullType>(get_cNull());
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

//    cInt->methods.insert({
//       "add",
//       make_nf_op_t(get_int_t(), {get_int_t()})
//    });

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

type_ptr class_has_method(const type_ptr & type, const std::string & method_name, const func_t_ptr & signature) {
    const auto & eq_range = type->_class->methods.equal_range(method_name);

    for (auto it = eq_range.first; it != eq_range.second; it++) {
        if (it->second->compare(signature)) {
            return it->second->return_type;
        }
    }

    return nullptr;
}
