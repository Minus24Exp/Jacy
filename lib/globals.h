#ifndef GLOBALS_H
#define GLOBALS_H

#include "vm/object.h"
#include "compiler/type.h"

#include <map>

namespace jc::globals {
    namespace G {
        // TODO: Hate yourself
        using namespace compiler;
        //

        // TODO: ? Kind ?
        struct Global {
            VarDeclKind kind;
            compiler::type_ptr type;
            vm::object_ptr value;
        };

        // print //
        const compiler::func_t_ptr print_signature = FuncType::get_nf_t(UnitType::get(), {FuncParamType::get(VarargType::get(Any::get()), false)});
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
        {"print", {VarDeclKind::Val, G::print_signature, make_nf("print", G::print)}},
    };
}

#endif // GLOBALS_H
