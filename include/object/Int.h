#ifndef INT_H
#define INT_H

#include "object/Object.h"

class Int : public Object {
public:
    Int(int64_t value) : value(value) {}
    virtual ~Int() = default;
private:
    int64_t value;
};

#endif
