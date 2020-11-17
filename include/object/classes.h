#ifndef CLASSES_H
#define CLASSES_H

#include "tree/nodes.h"
#include "compiler/types.h"

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
    std::map<std::string, func_t_ptr> methods;
    std::map<std::string, Field> fields;
};

struct cNull {};
struct cBool {};
struct cInt {};
struct cFloat {};
struct cString {};
struct cFunc {};
struct cUnion {};

#endif // CLASSES_H
