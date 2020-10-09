#ifndef BOOL_H
#define BOOL_H

#include "object/Object.h"
#include "object/Class.h"

class Bool;
using bool_ptr = std::shared_ptr<Bool>;

extern class_ptr cBool;

class Bool : public Object {
public:
    Bool(bool value) : Object(cBool), value(value) {}
    virtual ~Bool() = default;

private:
    bool value;
};

const bool_ptr FalseConst = std::make_shared<Bool>(false);
const bool_ptr TrueConst = std::make_shared<Bool>(true);

#endif
