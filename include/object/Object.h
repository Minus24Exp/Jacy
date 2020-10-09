#ifndef OBJECT_H
#define OBJECT_H

#include "vm/Value.h"

class Class;
using class_ptr = std::shared_ptr<Class>;

extern class_ptr cObject;

class Object {
public:
    Object(const class_ptr & _class) : _class(_class) {}
    virtual ~Object() = default;

private:
    class_ptr _class;
    std::map<std::string, Value> fields;
};
    
#endif
