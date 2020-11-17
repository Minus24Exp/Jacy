#ifndef OBJECT_H
#define OBJECTS_H

#include "vm/Value.h"
#include "compiler/types.h"

#include <map>

/**
 * objects is run-time structure
 * It mirrors compile-time Type object that contains too
 */

struct Object {
    class_ptr _class;
    std::map<std::string, Field> fields;

    bool has_field(const std::string & name) {
        return fields.find(name) != fields.end() || _class->methods.find(name) != _class->methods.end();
    }

    // TODO
    // value_ptr get();
};

#endif // OBJECT_H
