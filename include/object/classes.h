#ifndef CLASSES_H
#define CLASSES_H

#include "tree/nodes.h"
#include "compiler/types.h"

/**
 * Classes describes compile-time signatures
 */

struct Field {
    VarDeclKind kind;
    value_ptr value;
    std::string name;
};

struct Class {
    // TODO: ! Rewrite for any function !
    nf_ptr constructor;
    std::map<std::string, nf_ptr> methods;
    std::map<std::string, value_ptr> fields;
};

#endif // CLASSES_H
