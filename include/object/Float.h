#ifndef FLOAT_H
#define FLOAT_H

#include "object/NativeFunc.h"

class Float;
extern std::shared_ptr<Class> cFloat;
using float_ptr = std::shared_ptr<Float>;
extern std::unordered_map<yo_int, int_ptr> int_constants;

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

inline float_ptr make_float(double value){
    if(float_constants.find(value) != float_constants.end()){
        return float_constants.at(value);
    }
    float_ptr new_float = std::make_shared<Float>(value);
    float_constants.emplace(value, new_float);
    return new_float;
}

#endif