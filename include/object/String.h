#ifndef STRING_H
#define STRING_H

#include "object/NativeFunc.h"

class String;
using string_ptr = std::shared_ptr<String>;

const auto cast_to_s = [](obj_ptr s) -> string_ptr { return std::dynamic_pointer_cast<String>(s); };

extern std::shared_ptr<Class> cString;

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