#ifndef VALUE_H
#define VALUE_H

#include <string>

struct Value {
    std::string to_string() const {
        return "(Some value)";
    }
};

#endif // VALUE_H
