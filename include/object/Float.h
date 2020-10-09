#ifndef FLOAT_H
#define FLOAT_H

#include "object/Object.h"

class Float;
using float_ptr = std::shared_ptr<Float>;

extern class_ptr cFloat;

class Float : public Object {
public:
    Float(double value) : Object(cFloat), value(value) {}
    virtual ~Float() = default;

private:
    double value;
};

std::map<double, float_ptr> float_table;

float_ptr make_float(double value) {
    if (float_table.find(value) != float_table.end()) {
        return float_table.at(value);
    }

    return float_table[value] = std::make_shared<Float>(value);
}

#endif
