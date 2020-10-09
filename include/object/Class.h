#ifndef CLASS_H
#define CLASS_H

#include "object/Object.h"

extern class_ptr cClass;

class Class : public Object {
public:
    Class(const class_ptr & super) : Object(cClass) {}
    virtual ~Class() = default;

private:
    std::map<std::string, Value> fields;
    class_ptr super;
};

#endif
