#ifndef INT_H
#define INT_H

#include "object/NativeFunc.h"
#include <iostream>

class Int;
class Float;

using int_ptr = std::shared_ptr<Int>;

extern std::shared_ptr<Class> cInt;
extern std::unordered_map<int, int_ptr> int_constants;

const auto cast_to_i = [](obj_ptr obj){ return std::dynamic_pointer_cast<Int>(obj); };

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

static inline int_ptr make_int(yo_int value){
    if(int_constants.find(value) != int_constants.end()){
        return int_constants.at(value);
    }
    int_ptr new_int = std::make_shared<Int>(value);
    int_constants.emplace(value, new_int);
    return new_int;
}

#endif