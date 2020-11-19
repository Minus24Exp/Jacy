#include "compiler/class.h"

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

func_t_ptr get_func_t(TypeTag callable_type, const type_ptr & return_type, const t_list & arg_types) {
    return std::make_shared<FuncType>(callable_type, return_type, arg_types, get_cFunc());
}
