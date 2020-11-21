#ifndef GLOBALS_H
#define GLOBALS_H

#include "vm/Object.h"
#include "compiler/class.h"

#include <map>

namespace G {
    // TODO: ? Kind ?
    struct Global {
        type_ptr type;
        object_ptr value;
    };

    // print //
    const func_t_ptr print_signature = make_nf_t(get_void_t(), t_list{make_vararg_t(get_any_t())});
    static object_ptr print(const FuncArgs & args) {
        for (int i = 0; i < args.size(); i++) {
            std::cout << args.at(i)->to_string();
            if (i < args.size() - 1) {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
        return nullptr;
    }
}

const std::map<std::string, G::Global> jcGlobals = {
    {"print", {G::print_signature, make_nf("print", G::print)}},
};

#endif // GLOBALS_H
