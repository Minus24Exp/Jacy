#ifndef BOOL_H
#define BOOL_H

#include "object/Object.h"

class Bool : public Object {
public:
    Bool(bool value) : value(value) {}
    virtual ~Bool() = default;
private:
    bool value;
};

const Value FalseConst = Value{Type::Bool, std::make_shared<Bool>(false)};
const Value TrueConst = Value{Type::Bool, std::make_shared<Bool>(true)};

#endif
