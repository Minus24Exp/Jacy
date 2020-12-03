#ifndef TYPE_CLASS_H
#define TYPE_CLASS_H

#include "tree/nodes.h"
#include "compiler/type.h"

namespace jc::compiler {

    // Helpers //
    func_t_ptr make_func_t(const type_ptr & return_type, const t_list & arg_types, bool is_operator = false, TypeTag callable_type = TypeTag::Func);
    func_t_ptr make_nf_t(const type_ptr & return_type, const t_list & arg_types, bool is_operator = false);
    func_t_ptr make_nf_op_t(const type_ptr & return_type, const t_list & arg_types);

    type_ptr make_vararg_t(const type_ptr & vararg_type);

    func_t_ptr class_has_method(const type_ptr & type, const std::string & method_name, const func_t_ptr & signature, bool is_op_optional);

    // Mangling //
    std::string mangle_type(const type_ptr & type, const std::string & name);
    std::string mangle_type(const type_ptr & type);
    std::string mangle_type(const func_t_ptr & func_t);
}

#endif // TYPE_CLASS_H
