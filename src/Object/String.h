#ifndef STRING_H
#define STRING_H

#include "object/NativeFunc.h"

class String;
const auto cast_to_s = [](obj_ptr s){ return std::dynamic_pointer_cast<String>(s); };

class String : public Object {
public:
    String(const std::string & s);
    virtual ~String() = default;

    std::string repr() const override {
        return "<String:'"+ value +"'>";
    }

    std::string get_value() const {
        return value;
    }

private:
    std::string value;
};

#endif