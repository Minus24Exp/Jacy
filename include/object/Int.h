#ifndef INT_H
#define INT_H

#include "object/Object.h"

class Int;
using int_ptr = std::shared_ptr<Int>;

extern class_ptr cInt;

class Int : public Object {
public:
    Int(int64_t value) : Object(cInt), value(value) {}
    virtual ~Int() = default;
private:
    int64_t value;
};

std::map<int64_t, int_ptr> int_table;

int_ptr make_int(int64_t value) {
    if (int_table.find(value) != int_table.end()) {
        return int_table.at(value);
    }

    return int_table[value] = std::make_shared<Int>(value);
}

#endif
