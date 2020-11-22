#ifndef CLASS_H
#define CLASS_H

#include "vm/object.h"

namespace jc::vm {
    struct Class {
        Class(std::string name) : name(std::move(name)) {}

        std::string name;
        std::map<std::string, object_ptr> fields;
        std::map<std::string, func_ptr> methods;
    };
}

#endif // CLASS_H
