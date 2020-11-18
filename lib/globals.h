#ifndef GLOBALS_H
#define GLOBALS_H

#include "vm/Value.h"
#include "compiler/type.h"
#include <map>

namespace G {
    // print //
    const func_t_ptr print_signature = std::make_shared<FuncType>(TypeTag::NativeFunc, void_t, t_list{any_t});
    static value_ptr print(const FuncArgs & args) {
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
    value_ptr value;
};

const std::map<std::string, Global> jcGlobals = {
    {"print", {G::print_signature, make_nf("print", G::print)}},
};

#endif // GLOBALS_H
