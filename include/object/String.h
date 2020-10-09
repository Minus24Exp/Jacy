#ifndef STRING_H
#define STRING_H

#include "object/Object.h"

class String;
using string_ptr = std::shared_ptr<String>;

extern class_ptr cString;

class String : public Object {
public:
    String(const std::string & value) : Object(cString), value(value) {}
    virtual ~String() = default;

private:
    std::string value;
};

std::map<std::string, string_ptr> string_table;

string_ptr make_string(const std::string & value) {
    if (string_table.find(value) != string_table.end()) {
        return string_table.at(value);
    }

    return string_table[value] = std::make_shared<String>(value);
}

#endif
