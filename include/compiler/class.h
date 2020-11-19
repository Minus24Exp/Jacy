#ifndef CLASS_H
#define CLASS_H

#include "tree/nodes.h"
#include "compiler/type.h"

/**
 * Classes describes compile-time signatures
 */

struct Field {
    VarDeclKind kind;
    type_ptr type;
    std::string name;
};

struct Class {
    // TODO: ! Rewrite for any function !
    func_t_ptr constructor;
    std::map<std::string, Field> fields;
    std::map<std::string, func_t_ptr> methods;
};

type_ptr get_any_t();
type_ptr get_null_t();
type_ptr get_void_t();
type_ptr get_bool_t();
type_ptr get_int_t();
type_ptr get_float_t();
type_ptr get_string_t();
func_t_ptr get_func_t(TypeTag callable_type, const type_ptr & return_type, const t_list & arg_types);
type_ptr make_vararg_t(const type_ptr & vararg_type);

#endif // CLASS_H
