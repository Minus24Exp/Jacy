#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "object/Object.h"

class Null : public Object {
public:
    Null(){}
    virtual ~Null() = default;

    std::string repr() const override {
        return "<Null>";
    }
};

const auto null_obj = std::make_shared<Null>();

class Int : public Object {
public:
    Int(int i) : value(i) {}
    virtual ~Int() = default;

    std::string repr() const override {
        return "<Int:"+ std::to_string(value) +">";
    }

private:
    int value;
};

#endif