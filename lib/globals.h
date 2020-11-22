#ifndef GLOBALS_H
#define GLOBALS_H

#include "vm/object.h"
#include "compiler/type_class.h"

#include <map>

namespace jc::globals {
    namespace G {
        // TODO: ? Kind ?
        struct Global {
            compiler::type_ptr type;
            vm::object_ptr value;
        };

        // print //
        const compiler::func_t_ptr print_signature = make_nf_t(compiler::get_void_t(), compiler::t_list{make_vararg_t(compiler::get_any_t())});
        static vm::object_ptr print(const vm::FuncArgs & args) {
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
}

#endif // GLOBALS_H
