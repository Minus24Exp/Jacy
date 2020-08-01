#ifndef STRING_H
#define STRING_H

#include "object/NativeFunc.h"

class String;
extern std::shared_ptr<Class> cString;
using string_ptr = std::shared_ptr<String>;
extern std::unordered_map<std::string, string_ptr> string_constants;

const auto cast_to_s = [](obj_ptr s) -> string_ptr { return std::dynamic_pointer_cast<String>(s); };

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

inline string_ptr make_string(std::string value){
    if(string_constants.find(value) != string_constants.end()){
        return string_constants.at(value);
    }
    string_ptr new_string = std::make_shared<String>(value);
    string_constants.emplace(value, new_string);
    return new_string;
}

#endif