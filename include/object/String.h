#ifndef STRING_H
#define STRING_H

#include "object/Object.h"

class String;
using string_ptr = std::shared_ptr<String>;

class String : public Object {
public:
    String(const char * value) : value(value) {}
    virtual ~String() = default;

private:
    const char * value;
};

const std::map<const char*, string_ptr> string_table;

string_ptr make_string(const char * value) {
    if (string_table.find(value) != string_table.end()) {
        return string_table.at(value);
    }

    return string_table[value] = std::make_shared<String>(value);
}

#endif
