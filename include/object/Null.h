#ifndef NULL_H
#define NULL_H

#include "object/Object.h"

class Null : public Object {
public:
    Null() {}
    virtual ~Null() = default;

private:
};

const Value NullConst = Value{Type::Null, std::make_shared<Null>()};

#endif
