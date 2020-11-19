#ifndef GLOBALS_H
#define GLOBALS_H

#include "vm/Object.h"
#include "compiler/class.h"

#include <map>

namespace G {
    // print //
    const func_t_ptr print_signature = get_func_t(TypeTag::NativeFunc, get_void_t(), t_list{get_any_t()});
    static object_ptr print(const FuncArgs & args) {
        for (const auto & arg : args) {
            std::cout << arg->to_string();
        }
        std::cout << std::endl;
        return nullptr;
    }
}

// TODO: ? Kind ?
struct Global {
    type_ptr type;
    object_ptr value;
};

const std::map<std::string, Global> jcGlobals = {
    {"print", {G::print_signature, make_nf("print", G::print)}},
};

#endif // GLOBALS_H
