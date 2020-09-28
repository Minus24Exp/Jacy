#ifndef VALUE_H
#define VALUE_H

#include <variant>

enum class ValueType {
    Null,
    Bool,
    Int,
    Float,
    String,
};

struct Value {
    ValueType type;
    std::variant<std::monostate, bool, long, double, std::string> as;
};

#endif
