#ifndef FLOAT_H
#define FLOAT_H

#include "object/NativeFunc.h"

class Float : public Object {
public:
    Float(double d);
    virtual ~Float() = default;

    std::string repr() const override {
        return "<Float:"+ std::to_string(value) +">";
    }

    double get_value() const {
        return value;
    }

private:
    double value;
};

#endif