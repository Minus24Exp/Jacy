#ifndef NULL_H
#define NULL_H

#include "object/Object.h"

extern class_ptr cNull;

class Null : public Object {
public:
    Null() : Object(cNull) {}
    virtual ~Null() = default;

private:
};

const std::shared_ptr<Null> NullConst = std::make_shared<Null>();

#endif
