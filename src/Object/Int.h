#ifndef INT_H
#define INT_H

#include "object/NativeFunc.h"

class Int;
const auto cast_to_i = [](obj_ptr i){ return std::dynamic_pointer_cast<Int>(i); };

class Int : public Object {
public:
    Int(int i);
    virtual ~Int() = default;

    std::string repr() const override {
        return "<Int:"+ std::to_string(value) +">";
    }

    int get_value() const {
        return value;
    }

private:
    int value;
};

#endif