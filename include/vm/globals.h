#ifndef GLOBALS_H
#define GLOBALS_H

#include "vm/Value.h"
#include <map>

namespace G {
    static value_ptr print(const FuncArgs & args) {
        for (const auto & arg : args) {
            std::cout << arg->to_string();
        }
        std::cout << std::endl;
        return nullptr;
    }
}

const std::map<std::string, value_ptr> jcGlobals = {
    {"print", make_nf("print", G::print)},
};

#endif // GLOBALS_H
