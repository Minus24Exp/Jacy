#ifndef NULL_H
#define NULL_H

#include "object/NativeFunc.h"

class Null : public Object {
public:
    Null();
    virtual ~Null() = default;

    std::string repr() const override {
        return "<Null>";
    }
};

const auto null_obj = std::make_shared<Null>();

#endif