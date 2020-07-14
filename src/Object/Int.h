#ifndef INT_H
#define INT_H

#include "object/NativeFunc.h"
#include "Token.h"

class Int;
const auto cast_to_i = [](obj_ptr i){ return std::dynamic_pointer_cast<Int>(i); };

class Float;

class Int : public Object {
public:
    Int(yo_int i);
    virtual ~Int() = default;

    std::string repr() const override {
        return "<Int:"+ std::to_string(value) +">";
    }

    yo_int get_value() const {
        return value;
    }

    std::shared_ptr<Float> to_float() const;

private:
    yo_int value;
};

#endif